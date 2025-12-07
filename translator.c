#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_LINE 1024
#define MAX_CLASSES 16
#define MAX_FIELDS 32
#define MAX_TOKEN 128
#define MAX_VARS 128

typedef struct {
    char type[MAX_TOKEN];
    char name[MAX_TOKEN];
} Field;

typedef struct {
    char name[MAX_TOKEN];
    Field fields[MAX_FIELDS];
    int field_count;
} ClassDef;

typedef struct {
    ClassDef classes[MAX_CLASSES];
    int class_count;
    char current_class[MAX_TOKEN];
} TranspilerState;

TranspilerState state = { .class_count = 0, .current_class = "" };

typedef struct {
    char name[MAX_TOKEN];
    char type[MAX_TOKEN];
} VarEntry;

VarEntry var_table[MAX_VARS];
int var_count = 0;

char* trim(char* s) {
    if (!s) return s;
    while (isspace((unsigned char)*s)) s++;
    if (*s == 0) return s;
    char* e = s + strlen(s) - 1;
    while (e > s && isspace((unsigned char)*e)) e--;
    e[1] = '\0';
    return s;
}

bool starts_with(const char* pre, const char* s) {
    if (!pre || !s) return false;
    size_t n = strlen(pre);
    return strncmp(pre, s, n) == 0;
}

void map_type(const char* dart_type, char* out) {
    if (strcmp(dart_type, "int") == 0) strcpy(out, "int");
    else if (strcmp(dart_type, "double") == 0) strcpy(out, "double");
    else if (strcmp(dart_type, "bool") == 0) strcpy(out, "int");
    else if (strcmp(dart_type, "String") == 0) strcpy(out, "char*");
    else if (strcmp(dart_type, "void") == 0) strcpy(out, "void");
    else strcpy(out, dart_type);
}

void record_var(const char* name, const char* type) {
    if (!name || !type) return;
    for (int i = 0; i < var_count; i++) {
        if (strcmp(var_table[i].name, name) == 0) {
            strcpy(var_table[i].type, type);
            return;
        }
    }
    if (var_count < MAX_VARS) {
        strcpy(var_table[var_count].name, name);
        strcpy(var_table[var_count].type, type);
        var_count++;
    }
}

const char* lookup_var(const char* name) {
    if (!name) return NULL;
    for (int i = 0; i < var_count; i++) {
        if (strcmp(var_table[i].name, name) == 0) return var_table[i].type;
    }
    return NULL;
}

void parse_classes_pass(char* line_raw) {
    char line[MAX_LINE];
    strcpy(line, line_raw);
    char* s = trim(line);
    if (!s || *s == '\0') return;
    if (starts_with("class ", s)) {
        char name[MAX_TOKEN];
        sscanf(s, "class %s", name);
        char* b = strchr(name, '{');
        if (b) *b = '\0';
        int idx = state.class_count++;
        strcpy(state.classes[idx].name, name);
        state.classes[idx].field_count = 0;
        strcpy(state.current_class, name);
        return;
    }
    if (strcmp(s, "}") == 0 && strlen(state.current_class) > 0) {
        state.current_class[0] = '\0';
        return;
    }
    if (strlen(state.current_class) > 0) {
        if (strchr(s, ';') && !strchr(s, '(') && !strchr(s, '=') ) {
            char tmp[MAX_LINE];
            strcpy(tmp, s);
            char* semi = strchr(tmp, ';');
            if (semi) *semi = '\0';
            char type[MAX_TOKEN], name[MAX_TOKEN];
            if (sscanf(tmp, "%s %s", type, name) == 2) {
                ClassDef* c = &state.classes[state.class_count - 1];
                if (c->field_count < MAX_FIELDS) {
                    strcpy(c->fields[c->field_count].type, type);
                    strcpy(c->fields[c->field_count].name, name);
                    c->field_count++;
                }
            }
        }
    }
}

/* transform_this must be available before emit_print uses it */
void transform_this(char* dst, const char* src) {
    dst[0] = '\0';
    const char* p = src;
    while (*p) {
        if (strncmp(p, "this.", 5) == 0) {
            strcat(dst, "self->");
            p += 5;
        } else if (strncmp(p, "this->", 6) == 0) {
            strcat(dst, "self->");
            p += 6;
        } else {
            int n = strlen(dst);
            dst[n] = *p;
            dst[n+1] = '\0';
            p++;
        }
    }
}

/* emit_print: if content is quoted string -> printf("..."); else -> printf("%d\n", expr); */
void emit_print(FILE* out, char* content) {
    if (!content) return;
    char* t = trim(content);
    int len = strlen(t);
    if (len >= 2 && ((t[0] == '"' && t[len-1] == '"') || (t[0] == '\'' && t[len-1] == '\''))) {
        const char* start = t + 1;
        int slen = len - 2;
        fprintf(out, "    printf(\"%.*s\\n\");\n", slen, start);
        return;
    }
    /* not a string literal: treat as expression / variable
       transform 'this.' -> 'self->' before emitting */
    char transformed[MAX_LINE];
    transform_this(transformed, t);
    /* if transformed contains quotes (unlikely) fall back to string */
    if (strchr(transformed, '"') || strchr(transformed, '\'')) {
        fprintf(out, "    printf(\"%s\\n\");\n", transformed);
    } else {
        fprintf(out, "    printf(\"%%d\\n\", %s);\n", transformed);
    }
}

bool line_ends_with_semicolon(const char* s) {
    int n = strlen(s);
    if (n == 0) return false;
    int i = n-1;
    while (i >= 0 && isspace((unsigned char)s[i])) i--;
    if (i >= 0 && s[i] == ';') return true;
    return false;
}

void process_line(FILE* out, char* line_raw) {
    char line[MAX_LINE];
    strcpy(line, line_raw);
    char* s = trim(line);
    if (!s) return;
    if (starts_with("//", s)) return;
    if (starts_with("import ", s)) return;

    static bool in_method = false;
    static int brace_depth = 0;
    static char current_class[MAX_TOKEN] = "";
    static char current_method_header[MAX_LINE] = "";

    if (starts_with("class ", s) && !in_method) {
        char cname[MAX_TOKEN];
        sscanf(s, "class %s", cname);
        char* b = strchr(cname, '{');
        if (b) *b = '\0';
        strcpy(current_class, cname);
        return;
    }

    if (strcmp(s, "}") == 0 && !in_method && strlen(current_class) > 0) {
        current_class[0] = '\0';
        return;
    }

    if (strlen(current_class) > 0 && !in_method) {
        if (strchr(s, '(') && strchr(s, '{')) {
            char tmp[MAX_LINE];
            strcpy(tmp, s);
            char* paren = strchr(tmp, '(');
            *paren = '\0';
            char* last_space = strrchr(tmp, ' ');
            if (!last_space) return;
            char func[MAX_TOKEN];
            strcpy(func, last_space + 1);
            *last_space = '\0';
            char ret[MAX_TOKEN];
            strcpy(ret, trim(tmp));

            char args_raw[MAX_LINE] = "";
            char* args_start = strchr(s, '(');
            if (args_start) {
                args_start++;
                char* args_close = strchr(args_start, ')');
                if (args_close) {
                    int alen = args_close - args_start;
                    strncpy(args_raw, args_start, alen);
                    args_raw[alen] = '\0';
                }
            }

            char c_ret[MAX_TOKEN];
            map_type(ret, c_ret);
            if (strlen(c_ret) == 0) strcpy(c_ret, "void");
            if (strlen(args_raw) > 0)
                snprintf(current_method_header, sizeof(current_method_header), "%s %s_%s(%s* self, %s) {\n", c_ret, current_class, func, current_class, args_raw);
            else
                snprintf(current_method_header, sizeof(current_method_header), "%s %s_%s(%s* self) {\n", c_ret, current_class, func, current_class);
            fprintf(out, "%s", current_method_header);
            in_method = true;
            brace_depth = 1;
            return;
        } else {
            return;
        }
    }

    if (in_method) {
        int opens = 0, closes = 0;
        for (char* p = s; *p; ++p) {
            if (*p == '{') opens++;
            else if (*p == '}') closes++;
        }
        brace_depth += opens - closes;

        if (strcmp(s, "}") == 0 && brace_depth <= 0) {
            fprintf(out, "}\n\n");
            in_method = false;
            brace_depth = 0;
            return;
        }

        if (starts_with("print(", s)) {
            char* content = strchr(s, '(') + 1;
            char* end = strrchr(content, ')');
            if (end) *end = '\0';
            emit_print(out, content);
            return;
        }

        char transformed[MAX_LINE];
        transform_this(transformed, s);

        if (!line_ends_with_semicolon(transformed)) {
            fprintf(out, "    %s;\n", transformed);
        } else {
            int len = strlen(transformed);
            while (len > 1 && transformed[len-1] == ';' && transformed[len-2] == ';') {
                transformed[len-1] = '\0';
                len--;
            }
            fprintf(out, "    %s\n", transformed);
        }
        return;
    }

    if (starts_with("void main", s)) {
        fprintf(out, "int main() {\n");
        return;
    }

    char* new_kw = strstr(s, " = new ");
    if (new_kw) {
        char left[MAX_LINE];
        strncpy(left, s, new_kw - s);
        left[new_kw - s] = '\0';
        char type[MAX_TOKEN], var[MAX_TOKEN];
        if (sscanf(left, "%s %s", type, var) >= 1) {
            char ctor[MAX_TOKEN] = "";
            char* after = new_kw + 7;
            int i = 0;
            while (after[i] && after[i] != '(' && !isspace((unsigned char)after[i]) && i < MAX_TOKEN-1) {
                ctor[i] = after[i];
                i++;
            }
            ctor[i] = '\0';
            fprintf(out, "    %s* %s = %s_new();\n", type, var, ctor[0] ? ctor : type);
            if (var[0]) record_var(var, type);
            return;
        }
    }

    char* dot = strchr(s, '.');
    char* openp = strchr(s, '(');
    char* semi = strrchr(s, ';');
    if (dot && openp && semi && !strchr(s, '=') ) {
        int dotpos = dot - s;
        char var[MAX_TOKEN];
        strncpy(var, s, dotpos);
        var[dotpos] = '\0';
        char method_call[MAX_LINE];
        strcpy(method_call, dot + 1);
        char method[MAX_TOKEN];
        char args[MAX_LINE] = "";
        char* p = strchr(method_call, '(');
        if (!p) return;
        int mlen = p - method_call;
        strncpy(method, method_call, mlen);
        method[mlen] = '\0';
        char* argstart = p + 1;
        char* argclose = strchr(argstart, ')');
        if (argclose) {
            int alen = argclose - argstart;
            strncpy(args, argstart, alen);
            args[alen] = '\0';
        }
        const char* vt = lookup_var(var);
        const char* cls = vt ? vt : state.class_count > 0 ? state.classes[0].name : "Obj";
        if (strlen(args) == 0)
            fprintf(out, "    %s_%s(%s);\n", cls, method, var);
        else
            fprintf(out, "    %s_%s(%s, %s);\n", cls, method, var, args);
        return;
    }

    if (dot && strchr(s, '=') && !strchr(s, '(')) {
        int dotpos = dot - s;
        char var[MAX_TOKEN];
        strncpy(var, s, dotpos);
        var[dotpos] = '\0';
        char* after = dot + 1;
        char* eq = strchr(after, '=');
        if (!eq) return;
        int flen = eq - after;
        char field[MAX_TOKEN];
        strncpy(field, after, flen);
        field[flen] = '\0';
        trim(field);
        char rhs[MAX_LINE];
        strcpy(rhs, eq + 1);
        char* endsemi = strrchr(rhs, ';');
        if (endsemi) *endsemi = '\0';
        trim(rhs);
        fprintf(out, "    %s->%s = %s;\n", var, field, rhs);
        return;
    }

    if (starts_with("print(", s)) {
        char* content = strchr(s, '(') + 1;
        char* end = strrchr(content, ')');
        if (end) *end = '\0';
        emit_print(out, content);
        return;
    }

    fprintf(out, "    %s\n", s);
}


char* load_raw_file(const char* filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return NULL;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buffer = malloc(size + 1);
    fread(buffer, 1, size, fp);
    buffer[size] = '\0';
    fclose(fp);
    return buffer;
}

char* load_bumi_source(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: Could not open %s\n", filename);
        return NULL;
    }

    // determine file size
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (size < 0) {
        fclose(fp);
        return NULL;
    }

    // allocate generous space
    char *out = malloc(size * 2 + 2048);
    if (!out) {
        fclose(fp);
        return NULL;
    }

    out[0] = '\0';

    char line[2048];
    while (fgets(line, sizeof(line), fp)) {
        // strip newline
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        strcat(out, "\"");

        for (size_t i = 0; i < strlen(line); i++) {
            if (line[i] == '"')        strcat(out, "\\\"");
            else if (line[i] == '\\') strcat(out, "\\\\");
            else {
                size_t cur = strlen(out);
                out[cur] = line[i];
                out[cur+1] = '\0';
            }
        }

        strcat(out, "\\n\"\n");
    }

    fclose(fp);
    return out;
}

char* strip_bumi_extension(const char *filename) {
    const char *base = filename;

    // Get pointer to last slash (Unix '/' or Windows '\')
    const char *slash = strrchr(filename, '/');
    const char *backslash = strrchr(filename, '\\');

    if (slash && backslash)
        base = (slash > backslash) ? slash + 1 : backslash + 1;
    else if (slash)
        base = slash + 1;
    else if (backslash)
        base = backslash + 1;

    size_t len = strlen(base);

    // Check if filename ends with ".bumi"
    const char *ext = ".bumi";
    size_t ext_len = strlen(ext);

    if (len >= ext_len && strcmp(base + len - ext_len, ext) == 0) {
        len -= ext_len; // remove extension
    }

    // Allocate returned string
    char *out = malloc(len + 1);
    if (!out) return NULL;

    strncpy(out, base, len);
    out[len] = '\0';

    return out;
}   
    // CHAT HANDLED THE TRUNCATION METHOD


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename.bumi>\n", argv[0]);
        return 1;
    }
    char* bumi_source = load_raw_file(argv[1]);
    char* bumi_string_literal = load_bumi_source(argv[1]); 
    const char* name = strip_bumi_extension(argv[1]);
    strncat(name, ".c", sizeof(name) - strlen(name) - 1);
    
    FILE* out = fopen(name, "w");
    
    if (!out) return 1;

    char* src_copy = strdup(bumi_source);
    char* line = strtok(src_copy, "\n");
    while (line) {
        parse_classes_pass(line);
        line = strtok(NULL, "\n");
    }
    free(src_copy);

    fprintf(out, "// --- GENERATED C CODE ---\n");
    fprintf(out, "#include <stdio.h>\n");
    fprintf(out, "#include <stdlib.h>\n");
    fprintf(out, "#include <string.h>\n\n");

    for (int i = 0; i < state.class_count; i++) {
        ClassDef* c = &state.classes[i];
        fprintf(out, "typedef struct {\n");
        for (int j = 0; j < c->field_count; j++) {
            char ct[MAX_TOKEN];
            map_type(c->fields[j].type, ct);
            fprintf(out, "    %s %s;\n", ct, c->fields[j].name);
        }
        fprintf(out, "} %s;\n\n", c->name);
        fprintf(out, "%s* %s_new();\n\n", c->name, c->name);
    }

    for (int i = 0; i < state.class_count; i++) {
        ClassDef* c = &state.classes[i];
        fprintf(out, "%s* %s_new() {\n", c->name, c->name);
        fprintf(out, "    %s* instance = (%s*)malloc(sizeof(%s));\n", c->name, c->name, c->name);
        for (int j = 0; j < c->field_count; j++) {
            if (strcmp(c->fields[j].type, "int") == 0 || strcmp(c->fields[j].type, "double") == 0 || strcmp(c->fields[j].type, "bool") == 0)
                fprintf(out, "    instance->%s = 0;\n", c->fields[j].name);
            else
                fprintf(out, "    instance->%s = NULL;\n", c->fields[j].name);
        }
        fprintf(out, "    return instance;\n}\n\n");
    }

    state.current_class[0] = '\0';
    src_copy = strdup(bumi_source);
    line = strtok(src_copy, "\n");
    while (line) {
        char tmp[MAX_LINE];
        strcpy(tmp, line);
        process_line(out, tmp);
        line = strtok(NULL, "\n");
    }
    free(src_copy);

    fclose(out);
    return 0;
}
