#include "../common/AVMCommon.h"
#include "../common/opcodeDefinitions.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <cassert>
// #include <unistd.h>
#include <signal.h>
#include <string.h>
#include <limits.h>


/* this lets the source compile without afl-clang-fast/lto */
#ifndef __AFL_FUZZ_TESTCASE_LEN
  ssize_t       fuzz_len;
  unsigned char fuzz_buf[1024000];

  #define __AFL_FUZZ_TESTCASE_LEN fuzz_len
  #define __AFL_FUZZ_TESTCASE_BUF fuzz_buf
  #define __AFL_FUZZ_INIT() void sync(void);
  #define __AFL_LOOP(x) \
    ((fuzz_len = read(0, fuzz_buf, sizeof(fuzz_buf))) > 0 ? 1 : 0)
  #define __AFL_INIT() sync()
#endif


__AFL_FUZZ_INIT();


/* To ensure checks are not optimized out it is recommended to disable
   code optimization for the fuzzer harness main() */
#pragma clang optimize off
#pragma GCC optimize("O0")


//Function prototypes
void contract_0(Stack& s, BlockchainState& BS, EvalContext& ctx, vector<Txn>& TxnGroup, uint8_t currentTxn);
void check_suspicious_conditions(BlockchainState& BS, EvalContext& ctx, vector<Txn>& txnGroup, Stack& s);
void build_semantic_input(unsigned char* buf, BlockchainState& BS, EvalContext& ctx, vector<Txn>& txnGroup);
void setup_blockchain_initial_state(BlockchainState& BS);


// Execute a run through the contracts
void execute_contract_call(BlockchainState& BS, EvalContext& ctx, vector<Txn>& txnGroup)
{
  Stack s;
  // contract calls go here
  //FOR NOW
  contract_0(s, BS, ctx, txnGroup, 0);
  check_suspicious_conditions(BS, ctx, txnGroup, s);
}


// Prepare a call and execute it (construct input, execute contract)
void prepare_call(unsigned char *buf, BlockchainState& BS)
{
  // BlockchainState BS;
  EvalContext ctx;
  vector<Txn> txnGroup;

  //mutate blockchain state
  //TODO: this goes in the mutator! we will manage it there

  //fuzz blockchain state here (when applicable)
  //fuzz context here (when applicable)
  build_semantic_input(buf, BS, ctx, txnGroup);

  // execute the contract call with the prepared input and context
  execute_contract_call(BS, ctx, txnGroup);
}


void check_suspicious_conditions(BlockchainState& BS, EvalContext& ctx, vector<Txn>& txnGroup, Stack& s)
{
  assert(s.top().value != 0);
}


void build_semantic_input(unsigned char* buf, BlockchainState& BS, EvalContext& ctx, vector<Txn>& txnGroup)
{
  //TODO: move ALL this into custom mutator!!

  int buf_idx = 0;
  // BS = *(BlockchainState*)((void*)(buf[sizeof(BS)]));
  // ctx = *(EvalContext*)((void*)(buf[sizeof(ctx)]));
  txnGroup.resize(16);
  // txnGroup =  *(vector<Txn>*)((void*)(buf[sizeof(txnGroup)]));
  // txnGroup[0].ApplicationArgs[0].value = buf[0];
  
  txnGroup[0].apan = OnComplete(buf[0]%6);
  
  switch (buf[1]%6)
  {
    case 0:
		  txnGroup[0].ApplicationArgs[0] = {105,110,105,116};
	  break;

	  case 1:
		  txnGroup[0].ApplicationArgs[0] = {112,117,98,108,105,115,104,77,101,115,115,97,103,101};
	  break;
	
	  case 2:
      txnGroup[0].ApplicationArgs[0] = {110,111,112};
	  break;

	  case 3:
      txnGroup[0].ApplicationArgs[0] = {118,101,114,105,102,121,83,105,103,115};
	  break;

    case 4:
      txnGroup[0].ApplicationArgs[0] = {118,101,114,105,102,121,86,65,65};
    break;

    case 5:
      txnGroup[0].ApplicationArgs[0] = {103,111,118,101,114,110,97,110,99,101};
    break;
  }
  txnGroup[0].ApplicationArgs[1] = {buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]};
  txnGroup[0].ApplicationArgs[2] = {buf[8], buf[9], buf[10], buf[11], buf[12], buf[13]};
  txnGroup[0].ApplicationArgs[3] = {buf[14], buf[15], buf[16], buf[17], buf[18], buf[19]};
  txnGroup[0].ApplicationID = 1;
  txnGroup[0].NumAppArgs = 4;

  //load context stuff appropriately
  ctx.CurrentApplicationID = 1;
}


void setupBlockchainInitialState(BlockchainState& BS)
{
  //Deploy application (setup initial global state).
  //By running this, we assume a post-deployment base state.
  //Useful when the deployment init script is not something we are interested in fuzzing.

  vector<Txn> txnGroup;
  Stack s;
  EvalContext ctx;

  txnGroup.resize(1);
  txnGroup[0].ApplicationID = 0;

  contract_0(s, BS, ctx, txnGroup, 0);

  //TODO: a "plugin" to get state directly from chain
}




int main(int argc, char **argv) 
{
  ssize_t        len;                        /* how much input did we read? */
  unsigned char *buf;                        /* test case buffer pointer    */

#ifdef __AFL_HAVE_MANUAL_CONTROL
  __AFL_INIT();
#endif
  buf = __AFL_FUZZ_TESTCASE_BUF;  // this must be assigned before __AFL_LOOP!

  //custom initial setup
  BlockchainState BS;
  setupBlockchainInitialState(BS);

  while (__AFL_LOOP(UINT_MAX)) 
  {
    len = __AFL_FUZZ_TESTCASE_LEN;  // do not use the macro directly in a call!

    //TODO: special function to prepare input goes here
    prepare_call(buf, BS);
  }

  return 0;
}