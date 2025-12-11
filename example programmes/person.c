// --- GENERATED C CODE ---
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int age;
    int salary;
} Person;

Person* Person_new();

Person* Person_new() {
    Person* instance = (Person*)malloc(sizeof(Person));
    instance->age = 0;
    instance->salary = 0;
    return instance;
}

void Person_setAge(Person* self, int newAge) {
    self->age = newAge;
}

void Person_giveRaise(Person* self, int amount) {
    self->salary = self->salary + amount;
    printf("%d\n", self->salary);
}

    
int main() {
    String out = "Lets Go";
    puts(out);
Person* p = Person_new();
    p->age =  30;
    p->salary =  50000;
    Person_setAge(p, 31);
    for (int i = 0; i < 3; i++){
    Person_giveRaise(p, 50000);
    }
    
    puts("Done");
    return 0;
    }
