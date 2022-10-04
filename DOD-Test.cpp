/*

Copyright (c) 2022, Croitor Cristian

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*/

/*
I wrote this little program as an exemplification of how the DOD approach 
(and acutally just one or two concepts, not the whole principles) can be used
to optimize a critical path or other parts of the code.
This example assumes that you have a OOPClass or a DOD counterpart, each 
containing the same data.
There are three scenarios:
1. plain OOP without polimorfism
2. OOP with polimorfism
3. DOD approach.

The results are that the DOD approach is an order of magnitude faster than the
rest.
*/

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
public: 
    int someVar;
    int* somePointer;
    char* someString;
    int anoterVar;
    int* someOtherPointer;
    void* yetAnotherOne;
    void* test;
    void* test2;
    void* test3;
  
    bool shouldUpdate = false;
    double xPos = 0.001f;
    float yPos = 0.002f;
    float zPos = 0.003f;
};

class OOPClassBase
{
    public: 
        virtual bool* ShouldUpdate() = 0;
        virtual double* XPos() = 0;
        virtual float* YPos() = 0;
        virtual float* ZPos() = 0;
};

class OOPClassDerived : OOPClassBase
{
public:    
    int someVar;
    int* somePointer;
    char* someString;
    int anoterVar;
    int* someOtherPointer;
    void* yetAnotherOne;
    void* test;
    void* test2;
    void* test3;

    bool* ShouldUpdate()
    {
        return &shouldUpdate;
    }

    double* XPos()
    {
        return &xPos;
    }

    float* YPos()
    {
        return &yPos;
    }

    float* ZPos()
    {
        return &zPos;
    }

private: 
    bool shouldUpdate = false;
    double xPos = 0.001f;
    float yPos = 0.002f;
    float zPos = 0.003f;
};

struct DODItemPos;

struct DODItem
{
    int someVar;
    int* somePointer;
    char* someString;
    int anoterVar;
    int* someOtherPointer;
    void* yetAnotherOne;
    void* test;
    void* test2;
    void* test3;

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
    printf("WARNING: This program does not perform ANY error checking, if it crashes it ran out of memory.\r\nClosing big memory consumers like Chrome, etc might help.\r\n");
    OOPClass* arrayOfOOPClassInstances = new OOPClass[instanceCount];
    
    OOPClassBase** arrayOfOOPDerivedClassInstances = new OOPClassBase*[instanceCount];

    DODItemPos* arrayOfDODItemPos = new DODItemPos[instanceCount / 3];

    int dodIndex = 0;

    for (int i = 0; i < instanceCount; ++i)
    {
        OOPClassDerived* instanceDerived = new OOPClassDerived();

        if ((i % 3) == 0)
        {
            *instanceDerived->ShouldUpdate() = true;
            (*instanceDerived->XPos())++;
            (*instanceDerived->YPos())++;
            (*instanceDerived->ZPos())++;

            arrayOfOOPClassInstances[i].shouldUpdate = true;
            arrayOfOOPClassInstances[i].xPos++;
            arrayOfOOPClassInstances[i].yPos++;
            arrayOfOOPClassInstances[i].zPos++;

            arrayOfDODItemPos[dodIndex].xPos++;
            arrayOfDODItemPos[dodIndex].yPos++;
            arrayOfDODItemPos[dodIndex++].zPos++;
        }
        
        arrayOfOOPDerivedClassInstances[i] = (OOPClassBase*)instanceDerived;
    }


    // Here is the actual test, same operations on the same three elements for the entire collection

    double elapsed;
    printf("Checking %i OOP elements: \n", instanceCount);
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

    delete[] arrayOfOOPClassInstances;

    double elapsedThree;
    BEGIN_TIMED_BLOCK(testC);

    for (int i = 0; i < instanceCount; ++i)
    {
        if (*arrayOfOOPDerivedClassInstances[i]->ShouldUpdate())
        {
            *arrayOfOOPDerivedClassInstances[i]->XPos() = 123456.7f;
            *arrayOfOOPDerivedClassInstances[i]->YPos() = 7.654321f;
            *arrayOfOOPDerivedClassInstances[i]->ZPos() = 8.888888f;
        }
    }
    END_TIMED_BLOCK(testC, elapsedThree)
    printf("Timed OOP Derived: %.6f s\n", elapsedThree);

    for (int i = 0; i < instanceCount; ++i)
    {
        delete arrayOfOOPDerivedClassInstances[i];
    }
    delete[] arrayOfOOPDerivedClassInstances;

    double elapsedTwo;
    printf("Checking %i DOD elements: \n", dodIndex);
    BEGIN_TIMED_BLOCK(testB);
    for (int i = 0; i < dodIndex; ++i)
    {
        arrayOfDODItemPos[i].xPos = 123456.7f;
        arrayOfDODItemPos[i].yPos = 7.654321f;
        arrayOfDODItemPos[i].zPos = 8.888888f;
    }
    END_TIMED_BLOCK(testB, elapsedTwo)
    printf("Timed DOD        : %.6f s\n", elapsedTwo);

    return 0;
}
