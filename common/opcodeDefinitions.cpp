#include "AVMCommon.h"
#include <cassert>

#pragma once

inline void pushint(Stack& s, uint64_t value)
{
    s.push(value);
}

inline void bool_and(Stack& s)
{
    // if (s.get(0).byteslice.size() || s.get(1).byteslice.size()) 
    //     return; //HERE we need to panic

    StackValue v1 = s.pop();
    StackValue v2 = s.pop();
    s.push(bool(v1.value) == bool(v2.value));
}

inline void bool_or(Stack& s)
{
    // if (s.get(0).byteslice.size() || s.get(1).byteslice.size()) 
    //     return; //HERE we need to panic

    StackValue v1 = s.pop();
    StackValue v2 = s.pop();
    s.push(bool(v1.value) || bool(v2.value));
}

inline void bool_eq(Stack& s)
{
    // if (s.get(0).byteslice.size() || s.get(1).byteslice.size()) 
    //     return; //HERE we need to panic

    StackValue v1 = s.pop();
    StackValue v2 = s.pop();
    if(v1.value == v2.value) s.push(1);
    else s.push(0);
}

inline void bool_neq(Stack& s)
{
    // if (s.get(0).byteslice.size() || s.get(1).byteslice.size()) 
    //     return; //HERE we need to panic

    StackValue v1 = s.pop();
    StackValue v2 = s.pop();
    if(v1.value != v2.value) s.push(1);
    else s.push(0);
}

inline void bool_lt(Stack& s)
{
    // if (s.get(0).byteslice.size() || s.get(1).byteslice.size()) 
    //     return; //HERE we need to panic

    StackValue v1 = s.pop();
    StackValue v2 = s.pop();
    if(v1.value < v2.value) s.push(1);
    else s.push(0);
}

inline void bool_gt(Stack& s)
{
    // if (s.get(0).byteslice.size() || s.get(1).byteslice.size()) 
    //     return; //HERE we need to panic
    
    StackValue v1 = s.pop();
    StackValue v2 = s.pop();
    if(v1.value < v2.value) s.push(1);
    else s.push(0);
}

inline void bool_leq(Stack& s)
{
    // if (s.get(0).byteslice.size() || s.get(1).byteslice.size()) 
    //     return; //HERE we need to panic
    
    StackValue v1 = s.pop();
    StackValue v2 = s.pop();
    if(v1.value <= v2.value) s.push(1);
    else s.push(0);
}

inline void bool_geq(Stack& s)
{
    // if (s.get(0).byteslice.size() || s.get(1).byteslice.size()) 
    //     return; //HERE we need to panic
    
    StackValue v1 = s.pop();
    StackValue v2 = s.pop();
    if(v1.value >= v2.value) s.push(1);
    else s.push(0);
}

inline void add(Stack& s)
{
    // if (s.get(0).byteslice.size() || s.get(1).byteslice.size()) 
    //     return; //HERE we need to panic
    
    StackValue v1 = s.pop();
    StackValue v2 = s.pop();
    s.push(v1.value+v2.value);
}

inline void sub(Stack& s)
{
    StackValue v1 = s.pop();
    StackValue v2 = s.pop();

    // if (v1.byteslice.size() || v2.byteslice.size()) 
    //     return; //HERE we need to panic

    assert(v1.value <= v2.value);
    // if (v1.value > v2.value) 
    //     return; //HERE we need to panic (underflow)
    
    s.push(v1.value-v2.value);
}

inline void mul(Stack& s)
{
    StackValue v1 = s.pop();
    StackValue v2 = s.pop();

    // if (v1.byteslice.size() || v2.byteslice.size()) 
    //     return; //HERE we need to panic
    
    s.push(v1.value*v2.value);
}

inline void mod(Stack& s)
{
    StackValue v1 = s.pop();
    StackValue v2 = s.pop();

    // if (v1.byteslice.size() || v2.byteslice.size()) 
    //     return; //HERE we need to panic
    
    s.push(v1.value%v2.value);
}

inline void div(Stack& s)
{
    StackValue v1 = s.pop();
    StackValue v2 = s.pop();

    if (v2.byteslice.size() || v2.byteslice.size()) 
        return; //HERE we need to panic

    assert(v2.value != 0);
    // if (v2.value == 0)
    //     return; //HERE we need to panic
    
    s.push(v1.value/v2.value);
}

inline void btoi(Stack& s)
{
    StackValue v1 = s.pop();
    uint64_t value =  (uint64_t)(v1.byteslice[0]) |
   (uint64_t)(v1.byteslice[1]) << 8  |
   (uint64_t)(v1.byteslice[2]) << 16 |
   (uint64_t)(v1.byteslice[3]) << 24 |
   (uint64_t)(v1.byteslice[4]) << 32 |
   (uint64_t)(v1.byteslice[5]) << 40 |
   (uint64_t)(v1.byteslice[6]) << 48 |
   (uint64_t)(v1.byteslice[7]) << 56;

   s.push(value);
}

inline void itob(Stack& s)
{
    StackValue v1 = s.pop();
    v1.byteslice.resize(8);
    v1.byteslice[0] = v1.value & 0x0000000f;
    v1.byteslice[1] = (v1.value>>8) & 0x0000000f;
    v1.byteslice[2] = (v1.value>>16) & 0x0000000f;
    v1.byteslice[3] = (v1.value>>24) & 0x0000000f;
    v1.byteslice[4] = (v1.value>>32) & 0x0000000f;
    v1.byteslice[5] = (v1.value>>40) & 0x0000000f;
    v1.byteslice[6] = (v1.value>>48) & 0x0000000f;
    v1.byteslice[7] = (v1.value>>56) & 0x0000000f;
    v1.value = 0;
    s.push(v1);
}

inline void load(Stack& s, EvalContext& ctx, uint64_t val)
{
    s.push(ctx.sp[val]);
}

inline void store(Stack& s, EvalContext& ctx, uint64_t val)
{   
    StackValue v1 = s.pop();
    ctx.sp[val] = v1;
}

inline void avm_assert(Stack& s)
{
    StackValue v1 = s.pop();
    assert(v1.value != 0);
    // if (v1.value == 0)
    //     return; //HERE we need to panic (explicit assert violation)
}

inline void avm_log(Stack& s)
{
    StackValue v1 = s.pop();
    //TODO: save log as an additional output. Conditionally discard.
    // (for cases when it is completely irrelevant for the fuzzing targets)
}

inline void dup(Stack& s)
{
    s.push(s.get(0));
}

inline void dupn(Stack& s, int N)
{
    for (int i = 0; i < N; i++) s.push(s.get(0));
}

inline void err()
{
    abort();
}

inline void pushbytes(Stack& s, string bytes)
{
    StackValue v1;
    for (char byte : bytes) v1.byteslice.push_back(byte);
    s.push(v1);
}

inline void pushbytes(Stack& s, uint64_t bytes)
{
    // TODO: rewrite
}

inline void pushbytes(Stack& s, vector<uint8_t>& bytes)
{
    StackValue v1(bytes);
    s.push(v1);
}

inline void pushbytes(Stack& s, std::initializer_list<uint8_t> bytes)
{
    StackValue v1(bytes);
    s.push(v1);
}

inline void bytec(Stack& s, EvalContext& ctx, uint8_t index)
{
    s.push(ctx.bytecblock[index]);
}

inline void intc(Stack& s, EvalContext& ctx, uint8_t index)
{
    s.push(ctx.intcblock[index]);
}

inline void concat(Stack& s)
{
    StackValue v1 = s.pop();
    s.top().byteslice.insert( s.top().byteslice.end(), v1.byteslice.begin(), v1.byteslice.end());
}

inline void pop(Stack& s)
{
    s.pop();
}

inline void popn(Stack& s, int N)
{
    for (int i = 0; i < N; i++) s.pop();
}

inline void app_global_put(Stack& s, BlockchainState& BS, EvalContext& ctx)
{
    vector<uint8_t> key = s.pop().byteslice;
    vector<uint8_t> val = s.pop().byteslice;
    BS.AppStorage[ctx.CurrentApplicationID].appGlobal[key] = val;
}

inline void app_global_get(Stack& s, BlockchainState& BS, EvalContext& ctx)
{
    vector<uint8_t> key = s.pop().byteslice;
    vector<uint8_t> val = s.pop().byteslice;
    s.push(BS.AppStorage[ctx.CurrentApplicationID].appGlobal[key]);
}

inline void len(Stack& s)
{
    s.push(s.top().byteslice.size());
}

enum txnField{Sender, Receiver, ApplicationID, GroupIndex, 
OnCompletion, ApprovalProgram, ClearStateProgram, RekeyTo, 
NumAppArgs, TypeEnum, Amount, Fee};
inline void txn_field(Stack& s, Txn& txn, txnField field)
{
    switch(field)
    {
        case Sender:
            s.push(txn.Sender);
            break;
        
        case ApplicationID:
            s.push(txn.ApplicationID);
            break;

        case GroupIndex:
            s.push(txn.GroupIndex);
            break;
        
        case OnCompletion:
            s.push(txn.apan);
            break;

        case ApprovalProgram:
            s.push(0);  //TODO: approval program (in bytecode)

        case ClearStateProgram:
            s.push(0);  //TODO: clear state program (in bytecode)
        
        case RekeyTo:
            s.push(0);  //TODO: rekey field (in bytecode)

        case NumAppArgs:
            s.push(txn.NumAppArgs);

        case TypeEnum:
            s.push(0);   //TODO: push typeenum

        default: break;
    }
}


enum txnaField{ApplicationArgs, Accounts};
inline void txna_field(Stack& s, Txn& txn, txnaField field, uint64_t index)
{
    switch(field)
    {
        case ApplicationArgs:
            s.push(txn.ApplicationArgs[index]);
            break;
        
        case Accounts:
            s.push(txn.Accounts[index]);
        break;
    }
}


enum globalField{ZeroAddress, LatestTimestamp, CreatorAddress, CurrentApplicationAddress, CurrentApplicationID};
inline void global_field(Stack& s, BlockchainState& BS, EvalContext& ctx, globalField field)
{
    switch(field)
    {
        case ZeroAddress:
            s.push(StackValue(BS.ZeroAddress, 32));
            break;
        
        case LatestTimestamp:
            s.push(StackValue(BS.LatestTimestamp));
            break;

        case CreatorAddress:
            s.push(StackValue(ctx.CreatorAddress, 32));
            break;

        case CurrentApplicationAddress:
            s.push(StackValue(ctx.CurrentApplicationAddress, 32));
            break;

        case CurrentApplicationID:
            s.push(StackValue(ctx.CurrentApplicationID));
            break;
    }
}

inline void itxn_begin(Stack& s, EvalContext& ctx)
{
    ctx.ActiveInners.push_back(Txn());
}

inline void itxn_field(Stack&s, EvalContext& ctx, txnField field)
{
    switch(field)
    {
        case TypeEnum:
            ctx.ActiveInners.back().TypeEnum = TxnType(s.pop().value);
            break;

        default:
            break;
    }
}

inline void itxn_next(Stack& s, EvalContext& ctx)
{
    ctx.ActiveInners.push_back(Txn());
}

inline void itxn_submit(BlockchainState& BS, EvalContext& ctx)
{
    ExecuteTransactionGroup(BS, ctx.ActiveInners);
    ctx.ActiveInners.clear();
}


inline void gtxn_field(Stack& s, vector<Txn>& TxnGroup, uint64_t index, txnField field)
{
    txn_field(s, TxnGroup[index], field);
}

inline void gtxn_field(Stack& s, vector<Txn>& TxnGroup, txnField field)
{
    uint64_t idx = s.pop().value;
    gtxn_field(s, TxnGroup, idx, field);
}

inline void gtxna_field(Stack& s, vector<Txn>& TxnGroup, txnaField field, uint64_t fieldIndex)
{
    uint64_t idx = s.pop().value;
    txna_field(s, TxnGroup[idx], field, fieldIndex);
}

inline void extract(Stack& s, uint64_t S, uint64_t L)
{
    vector<uint8_t> v1;
    if (L != 0) v1.insert(v1.begin(), s.top().byteslice.begin()+S, s.top().byteslice.begin()+S+L);
    else v1.insert(v1.begin(), s.top().byteslice.begin()+S, s.top().byteslice.end());
    s.pop();
    s.push(v1);
}

inline void extract3(Stack& s)
{
    uint64_t L = s.pop().value;
    uint64_t S = s.pop().value;
    extract(s, S, L);
}

inline void keccak256(Stack& s)
{
    //TODO
}

inline void sha512_256(Stack& s)
{
    //TODO
}

inline void bzero(Stack& s)
{
    uint64_t len = s.pop().value;
    s.push(StackValue());
    for (int i = 0; i < len; i++) s.top().byteslice.push_back(0);
}

inline void substring(Stack& s, uint64_t S, uint64_t E)
{
    assert(E >= S && E <= s.top().byteslice.size() && S <= s.top().byteslice.size());

    vector<uint8_t> v1;
    v1.insert(v1.begin(), s.top().byteslice.begin()+S, s.top().byteslice.begin()+E);
    s.pop();
    s.push(v1);
}

inline void substring3(Stack& s)
{
    uint64_t C = s.pop().value;
    uint64_t B = s.pop().value;
    substring(s, B, C);
}

inline void uncover(Stack& s, uint64_t depth)
{
    StackValue v1 = s.get(depth+1);
    for (int i = depth; i >= 0; i--) s.stack[i+1] = s.stack[i];
    s.top() = v1;
}

inline void cover(Stack& s, uint64_t depth)
{
    StackValue v1 = s.top();
    for (int i = 0; i <= depth; i++) s.stack[i] = s.stack[i+1];
    s.get(depth+1) = v1;
}

inline void app_local_put(Stack& s, BlockchainState& BS)
{
    //TODO
}

inline void app_local_get(Stack& s, BlockchainState& BS)
{
    //TODO
}

inline void getbyte(Stack& s)
{
    uint64_t ind = s.pop().value;
    assert(ind > s.top().byteslice.size());
    vector<uint8_t> v1 = s.pop().byteslice;

    s.push(StackValue(v1[ind]));
}

inline void setbyte(Stack& s)
{
    uint64_t value = s.pop().value;
    assert(value <= 255);
    
    uint64_t ind = s.pop().value;
    assert(ind < s.top().byteslice.size());

    s.top().byteslice[ind] = value;
}

inline void getbit(Stack& s)
{
    uint64_t index = s.pop().value;

}

inline void setbit(Stack& s)
{
    uint64_t index = s.pop().value;
}

inline void bitwise_and(Stack& s)
{
    uint64_t operand = s.pop().value;
    s.top().value &= operand;
}

inline void bitwise_or(Stack& s)
{
    uint64_t operand = s.pop().value;
    s.top().value |= operand;
}

inline void bitwise_shr(Stack& s)
{
    uint64_t n = s.pop().value;
    s.top().value >>= n;
}

inline void bitwise_shl(Stack& s)
{
    uint64_t n = s.pop().value;
    s.top().value <<= n;
}




// pseudo-opcodes
enum alias{a, b, c, d, e, f, appl};
inline void avm_int(Stack& s, uint64_t value)
{
    pushint(s, value);
}

inline void avm_int(Stack& s, alias value)
{
    pushint(s, value);
}

inline void avm_byte(Stack& s, string value)
{
    pushbytes(s, value);
}