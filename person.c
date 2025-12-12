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
    this.age = =  newAge;
}

void Person_giveRaise(Person* self, int amount) {
    char* man =  "man";
    this.salary = =  self->salary + amount;
    puts(man);
    printf("%d\n", self->salary);
}

    
int main() {
    char* out =  "Lets Go";
    puts(out);
    Person p =  new Person();
    p.age = =  30;
    p.salary = =  50000;
    p.setAge(31);
    for (int =  0; i < 3;
    p.giveRaise(50000);
    };
    ;
    puts("Done");
    return 0;
}

