#include <vector>
#include <map>
#include <string>

#pragma once

#define STACK_MAX_SIZE 1000

using namespace std;


//A value in the stack
struct StackValue
{
    vector<uint8_t> byteslice;
    uint64_t value;

    StackValue(){}
    StackValue(uint64_t i):value(i){}
    //que hacia esto en terminos de memoria? es un buen caso para move semantics?
    StackValue(vector<uint8_t>& b):byteslice(b){}
    StackValue(std::initializer_list<uint8_t> b):byteslice(b){}
    StackValue(uint8_t* b, int size)
    {
        byteslice.resize(size);
        for (int i = 0; i < size; i++) byteslice[i] = b[i];
    }

    inline bool operator ==(StackValue& a){ return value == a.value && byteslice == a.byteslice;}
};

struct balanceRecord
{
    
};

struct ApplicationState
{
    uint64_t ApplicationID;
    std::map<vector<uint8_t>, StackValue> appGlobal;

    //app stuff as if it was an account
    balanceRecord record;
};

//General blockchain state to provide a fuzzing context
struct BlockchainState
{
    //accounts (balances, ASA, local storage)

    //Application storage mapping
    std::map<uint64_t, ApplicationState> AppStorage;

    //global state
    uint8_t ZeroAddress[32];
    uint64_t LatestTimestamp;
};


enum OnComplete{NoOp, OptIn, CloseOut, ClearState, UpdateApplication, DeleteApplication};
enum TxnType{Pay, Appl};
struct Txn
{
    TxnType TypeEnum;
    StackValue Sender;
    OnComplete apan;
    uint64_t ApplicationID;
    uint64_t GroupIndex;

    vector<uint8_t> ApplicationArgs[16];
    StackValue Accounts[4];
    vector<StackValue> Assets;
    vector<StackValue> Applications;

    uint64_t NumAppArgs;
};


struct Stack
{
    uint32_t currentTop = 0;
    StackValue stack[1000];

    inline StackValue pop(){return stack[currentTop--];}
    inline StackValue& get(int i){return stack[i];}
    inline StackValue& top(){return get(0);} 
    // inline void push(StackValue& v){stack[currentTop++] = v;}
    inline void push(StackValue v){stack[currentTop++] = v;}
    inline void push(uint64_t i){stack[currentTop++] = StackValue(i);}
    inline void discard(uint64_t n){currentTop -= n;}
};


struct EvalContext
{
    Stack s; 
    StackValue sp[256]; //scratchspace

    // EvalContext(){}

    //Este se toca una sola vez y no cambia por ejecucion (en tamanio maximo)
    //Se puede directamente saber cual es el maximo que va a ocupar
    //  y metagenerarlo como optimizacion.
    vector<StackValue> ApplicationArgs;

    vector<int> CallsubStack;

    vector<uint64_t> intcblock;
    vector<vector<uint8_t>> bytecblock; 

    uint64_t CurrentApplicationID;

    //this goes here for now
    //later move to BS
    uint8_t CreatorAddress[32];
    uint8_t CurrentApplicationAddress[32];

    vector<Txn> ActiveInners;
};


void ExecuteTransaction(BlockchainState& BS, vector<Txn>& TxnGroup, int currentTransaction)
{

}


void ExecuteTransactionGroup(BlockchainState& BS, vector<Txn>& TxnGroup)
{
    //TODO: save eval delta
    for (int i = 0; i < TxnGroup.size(); i++)
        ExecuteTransaction(BS, TxnGroup, i);
}