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
    Person* p = Person_new();
    p->age =  30;
    p->salary =  50000;
    Person_setAge(p, 31);
    Person_giveRaise(p, 5000);
    return 0;
    };
