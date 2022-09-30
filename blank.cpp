#include <stdio.h>
#include <Windows.h>

#define BEGIN_TIMED_BLOCK(name) \
{LARGE_INTEGER ulibStartTimer##name;\
LARGE_INTEGER ulibFrequency##name;\
QueryPerformanceCounter(&ulibStartTimer##name);\
QueryPerformanceFrequency(&ulibFrequency##name);

#define END_TIMED_BLOCK(name, res) \
LARGE_INTEGER ulibStopTimer##name;\
QueryPerformanceCounter(&ulibStopTimer##name);\
res = (double)(ulibStopTimer##name.QuadPart - ulibStartTimer##name.QuadPart) /\
      (double)ulibFrequency##name.QuadPart;}

#define ONE_THOUSAND 1000
#define MILLION ONE_THOUSAND * ONE_THOUSAND

static int unsigned instanceCount = 10 * MILLION;

class OOPClass
{
    public: int someVar;
    public: int* somePointer;
    public: char* someString;
    public: int anoterVar;
    public: int* someOtherPointer;
    public: void* yetAnotherOne;

    public: bool shouldUpdate = false;
    
    public: double xPos = 0.001f;
    public: float yPos = 0.002f;
    public: float zPos = 0.003f;
}

class OOPClassBase
{
    virtual bool* ShouldUpdate() = 0;
    virtual double* XPos() = 0;
}

class OOPClassDerived : OOPClassBase
{
    public: int someVar;
    public: int* somePointer;
    public: char* someString;
    public: int anoterVar;
    public: int* someOtherPointer;
    public: void* yetAnotherOne;

    bool* ShouldUpdate()
    {
        return &shouldUpdate;
    }

    double* XPos()
    {
        return &xPos;
    }

    private: bool shouldUpdate = false;
    
    private: double xPos = 0.001f;
    public: float yPos = 0.002f;
    public: float zPos = 0.003f;
}

struct DODItemPos;

struct DODItem
{
    int someVar;
    int* somePointer;
    char* someString;
    int anoterVar;
    int* someOtherPointer;
    void* yetAnotherOne;

    DODItemPos* pos;
};

struct DODItemPos
{
    double xPos = 0.001f;
    float yPos = 0.002f;
    float zPos = 0.003f;
};


int main()
{
    OOPClass* arrayOfOOPClassInstances = new OOPClass[instanceCount];
    
    OOPClassDerived* arrayOfOOPDerivedClassInstances = new OOPClassDerived[instanceCount];

    DODItemPos* arrayOfDODItemPos = new DODItemPos[instanceCount];

    for (int i = 0; i < instanceCount; ++i)
    {
        OOPClass* instance = new OOPClass();
        OOPClassDerived* instanceDerived = new OOPClassDerived();
        DODItemPos* itemPos = new DODItemPos();

        if ((i % 3) == 0)
        {
            *instanceDerived->ShouldUpdate() = true;
            (*instanceDerived->XPos())++;
            instanceDerived->yPos++;
            instanceDerived->zPos++;

            instance->shouldUpdate = true;
            instance->xPos++;
            instance->yPos++;
            instance->zPos++;

            itemPos->xPos++;
            itemPos->yPos++;
            itemPos->zPos++;
        }
        else
        {
            (*instanceDerived->XPos()) += i;
            instanceDerived->yPos += i + 0.1f;
            instanceDerived->zPos += i + 0.2f;

            instance->xPos += i;
            instance->yPos += i + 0.1f;
            instance->zPos += i + 0.2f;

            itemPos->xPos += i;
            itemPos->yPos += i + 0.1f;
            itemPos->zPos += i + 0.2f;
        }

        arrayOfDODItemPos[i] = *itemPos;
        arrayOfOOPClassInstances[i] = *instance;
    }


    // Here is the actual test, same operations on the same three elements for the entire collection

    double elapsed;
    BEGIN_TIMED_BLOCK(test);
    for (int i = 0; i < instanceCount; ++i)
    {
        if (arrayOfOOPClassInstances[i].shouldUpdate)
        {
            arrayOfOOPClassInstances[i].xPos = 123456.7f;
            arrayOfOOPClassInstances[i].yPos = 7.654321f;
            arrayOfOOPClassInstances[i].zPos = 8.888888f;
        }
    }
    END_TIMED_BLOCK(test, elapsed);
    printf("Timed OOP        : %.6f s\n", elapsed);

    double elapsedThree;
    BEGIN_TIMED_BLOCK(testC);

    for (int i = 0; i < instanceCount; ++i)
    {
        (*arrayOfOOPDerivedClassInstances[i].XPos()) = 123456.7f;
        arrayOfOOPDerivedClassInstances[i].yPos = 7.654321f;
        arrayOfOOPDerivedClassInstances[i].zPos = 8.888888f;
    }
    END_TIMED_BLOCK(testC, elapsedThree)
    printf("Timed OOP Derived: %.6f s\n", elapsedThree);

    double elapsedTwo;
    BEGIN_TIMED_BLOCK(testB);
    for (int i = 0; i < instanceCount; ++i)
    {
        arrayOfDODItemPos[i].xPos = 123456.7f;
        arrayOfDODItemPos[i].yPos = 7.654321f;
        arrayOfDODItemPos[i].zPos = 8.888888f;
    }
    END_TIMED_BLOCK(testB, elapsedTwo)
    printf("Timed DOD        : %.6f s\n", elapsedTwo);

    return 0;
}
