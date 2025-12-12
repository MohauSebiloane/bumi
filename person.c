// --- GENERATED C CODE ---
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int age;
    int salary;
    char* name;
} Person;

Person* Person_new();

Person* Person_new() {
    Person* instance = (Person*)malloc(sizeof(Person));
    instance->age = 0;
    instance->salary = 0;
    instance->name = NULL;
    return instance;
}

void Person_setAge(Person* self, int newAge) {
    this.age  = newAge;;
}

void Person_giveRaise(Person* self, int amount) {
    char* man = "man";;
    this.salary  = this.salary + amount;;
    puts(man);
    printf("%d\n", self->salary);
}

    
int main() {
    String out = "Lets Go";
    print(out);
Person* p = Person_new();
    p->age =  30;
    p->salary =  50000;
    Person_setAge(p, 31);
    for (int i = 0; i < 3; i++){
    Person_giveRaise(p, 50000);
    }
    
    puts("Done");
    return 0;
    }a
