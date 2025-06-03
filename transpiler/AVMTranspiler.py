def parse_contract(filename:str):
    # Parses the given contract
    # Returns a list of parsed lines
    l = []
    with open(filename, 'r') as f:
        for line in f:
            #Strip away comments.
            #Then, separate all ';' finished lines
            #   (if there were any) into distinct
            #   program lines.

            # TODO: hack for a specific example. 
            # Better to just rewrite the parser based on the golang impl.
            if line.count("http") == 0:
                lines = line.split("//")[0].split(';')
            else:
                lines = [line]

            for k in lines:
                op = k.split()
                if len(op) > 0:
                    l.append(op)
    return l


def generate_callswitch(callsub_n:int) -> str:
    switch_call_out = "\tswitch (ctx.CallsubStack.back()){\n"
    for i in range(0, callsub_n):
        switch_call_out += "\t\tcase %d:\n" % i
        switch_call_out += "\t\tgoto callsub_%d;\n" % i
    switch_call_out += "\t}\n"
    return switch_call_out

def generate_match_branches(label_args:[str]) -> str:
    match_out = "\t"
    for i in range(1, len(label_args)+1):
        match_out += "if (s.top() == s.stack[%d]){ s.discard(%d); goto %s;}\n" % (i, len(label_args)+1, label_args[i-1])
        match_out += "\telse "
    match_out += "s.discard(%s);\n" % (len(label_args)+1)
    return match_out

def parse_hex_literal_as_init_list(hex_number: str) -> str:
    value = "{"
    hex_number = hex_number[2:] # get rid of 0x for Python
    if len(hex_number) == 0:
        value+='}'
        return value
    bytes = bytearray.fromhex(hex_number)
    for b in bytes:
        value += str(b) + ','
    value = value[:-1] + '}'
    return value

def unsupported_opcode(l:str):
    out = ""
    out = "\t//UNSUPPORTED OPCODE "
    for s in l:
        out += s + " "
    out += '\n'
    return out


def transpile_contract(parsed_contract:list[str]):
    transpiled_out = ""
    callsub_n = 0
    callsub_total = 0

    for l in parsed_contract:
        if l[0] == "callsub":
            callsub_total += 1

    for l in parsed_contract:
        if l[0] == "match":
            transpiled_out += generate_match_branches(l[1:])
        elif l[0] == "intcblock":
            init_list = '{'+",".join(([("%s"%n) for n in l[1:]]))+'}'
            transpiled_out += "\tctx.intcblock = %s;\n" % init_list
        elif l[0] == "bytecblock":
            init_list = '{'+",".join(([("%s"%parse_hex_literal_as_init_list(n)) for n in l[1:]]))+'}'
            transpiled_out += "\tctx.bytecblock = %s;\n" % init_list
        elif len(l) == 1:
            # zero argument opcodes
            if l[0] == "app_global_put":
                transpiled_out += "\tapp_global_put(s, BS, ctx);\n"
            elif l[0] == "app_global_get":
                transpiled_out += "\tapp_global_get(s, BS, ctx);\n"
            elif l[0] == "app_local_put":
                transpiled_out += "\tapp_local_put(s, BS);\n"
            elif l[0] == "app_local_get":
                transpiled_out += "\tapp_local_get(s, BS);\n"
            elif l[0] == "&":
                transpiled_out += "\tbitwise_and(s);\n"
            elif l[0] == "|":
                transpiled_out += "\tbitwise_or(s);\n"
            elif l[0] == "shr":
                transpiled_out += "\tbitwise_shr(s);\n"
            elif l[0] == "shl":
                transpiled_out += "\tbitwise_shl(s);\n"
            elif l[0] == "getbyte":
                transpiled_out += "\tgetbyte(s);\n"
            elif l[0] == "setbyte":
                transpiled_out += "\tsetbit(s);\n" 
            elif l[0] == "getbit":
                transpiled_out += "\tgetbit(s);\n"
            elif l[0] == "setbit":
                transpiled_out += "\tsetbyte(s);\n" 
            elif l[0] == "bzero":
                transpiled_out += "\tbzero(s);\n"
            elif l[0] == "itxn_begin":
                transpiled_out += "\titxn_begin(s,ctx);\n"
            elif l[0] == "itxn_submit":
                transpiled_out += "\titxn_submit(BS, ctx);\n"
            elif l[0] == "len":
                transpiled_out += "\tlen(s);\n"
            elif l[0] == "==":
                transpiled_out += "\tbool_eq(s);\n"
            elif l[0] == "!=":
                transpiled_out += "\tbool_neq(s);\n"
            elif l[0] == "<":
                transpiled_out += "\tbool_lt(s);\n"
            elif l[0] == ">":
                transpiled_out += "\tbool_gt(s);\n"
            elif l[0] == "<=":
                transpiled_out += "\tbool_leq(s);\n"
            elif l[0] == ">=":
                transpiled_out += "\tbool_geq(s);\n"
            elif l[0] == "&&":
                transpiled_out += "\tbool_and(s);\n"
            elif l[0] == "||":
                transpiled_out += "\tbool_or(s);\n"
            elif l[0] == "+":
                transpiled_out += "\tadd(s);\n"
            elif l[0] == "-":
                transpiled_out += "\tsub(s);\n"
            elif l[0] == "*":
                transpiled_out += "\tmul(s);\n"
            elif l[0] == "/":
                transpiled_out += "\tdiv(s);\n"
            elif l[0] == "%":
                transpiled_out += "\tmod(s);\n"
            elif l[0] == "assert":
                transpiled_out += "\tavm_assert(s);\n"
            elif l[0] == "log":
                transpiled_out += "\tavm_log(s);\n"
            elif l[0] == "btoi":
                transpiled_out += "\tbtoi(s);\n"
            elif l[0] == "err":
                transpiled_out += "\terr();\n"
            elif l[0] == "dup":
                transpiled_out += "\tdup(s);\n"
            elif l[0][-1] == ":":
                # its a tag
                transpiled_out += l[0]+"\n"
            elif l[0][0:5] == "intc_":
                split = l[0].split('_')
                transpiled_out += "\ts.push(ctx.intcblock[%s]);\n" % split[1]
            elif l[0][0:6] == "bytec_":
                split = l[0].split('_')
                transpiled_out += "\ts.push(ctx.bytecblock[%s]);\n" % split[1]
            elif l[0] == "retsub":
                transpiled_out += generate_callswitch(callsub_total)
            elif l[0] == "return":
                transpiled_out += "\treturn;\n"
            elif l[0] == "concat":
                transpiled_out += "\tconcat(s);\n"
            elif l[0] == "pop":
                transpiled_out += "\tpop(s);\n"
            elif l[0] == "itob":
                transpiled_out += "\titob(s);\n"
            elif l[0] == "extract3":
                transpiled_out += "\textract3(s);\n"
            elif l[0] == "substring3":
                transpiled_out += "\tsubstring3(s);\n"
            elif l[0] == "keccak256":
                transpiled_out += "\tkeccak256(s);\n"
            elif l[0] == "sha512_256":
                transpiled_out += "\tsha512_256(s);\n"
            else:
                transpiled_out += unsupported_opcode(l)
        elif len(l) == 2:
            #single argument opcodes
            if l[0] == "pushint":
                transpiled_out += "\tpushint(s, %s);\n" % l[1]
            elif l[0] == "pushbytes":
                value = ""
                if l[1][0] == "\"" or l[1][:2] != "0x":
                    value = l[1]
                else:
                    value = parse_hex_literal_as_init_list(l[1])
                transpiled_out += "\tpushbytes(s, %s);\n" % value
            elif l[0] == "int":
                transpiled_out += "\tavm_int(s, %s);\n" % l[1]
            elif l[0] == "byte":
                transpiled_out += "\tavm_byte(s, %s);\n" % l[1]
            elif l[0] == "b":
                transpiled_out += "\tgoto %s;\n" % l[1]
            elif l[0] == "bz":
                transpiled_out += "\tif(s.top().value == 0) goto %s;\n" % l[1]
            elif l[0] == "bnz":
                transpiled_out += "\tif(s.top().value != 0) goto %s;\n" % l[1]
            elif l[0] == "callsub":
                transpiled_out += "\tctx.CallsubStack.push_back(%d);\n" % callsub_n
                transpiled_out += "\tgoto %s;\n" % l[1]
                transpiled_out += "callsub_%d:\n" % callsub_n
                callsub_n += 1
            elif l[0] == "load":
                transpiled_out += "\tload(s, ctx, %s);\n" % l[1]
            elif l[0] == "store":
                transpiled_out += "\tstore(s, ctx, %s);\n" % l[1]
            elif l[0] == "popn":
                transpiled_out += "\tpopn(s, %d);\n" % l[1]
            elif l[0] == "dupn":
                transpiled_out += "\tdupn(s, %d);\n" % l[1]
            elif l[0] == "gtxns":
                transpiled_out += "\tgtxn_field(s, TxnGroup, %s);\n" % l[1]
            elif l[0] == "txn":
                transpiled_out += "\ttxn_field(s, TxnGroup[currentTxn], %s);\n" % l[1]
            elif l[0] == "global":
                transpiled_out += "\tglobal_field(s, BS, ctx, %s);\n" % l[1]
            elif l[0] == "intc":
                transpiled_out += "\tintc(s, ctx, %s);\n" % l[1]
            elif l[0] == "bytec":
                transpiled_out += "\tbytec(s, ctx, %s);\n" % l[1]
            elif l[0] == "cover":
                transpiled_out += "\tcover(s, %s);\n" % l[1]
            elif l[0] == "uncover":
                transpiled_out += "\tuncover(s, %s);\n" % l[1]
            elif l[0] == "itxn_field":
                transpiled_out += "\titxn_field(s, ctx, %s);\n" % l[1] 
            else:
                transpiled_out += unsupported_opcode(l)
        elif len(l) == 3:
            if l[0] == "#pragma":
                #For now, we ignore the pragma directive.
                #Maybe we should save it as a constant somehwere.
                pass
            elif l[0] == "#define":
                if l[2][0:2] == "0x":
                    transpiled_out += "#define %s %s\n" % (l[1], parse_hex_literal_as_init_list(l[2]))
                else:
                    transpiled_out += "#define %s %s\n" % (l[1], l[2].replace("_",""))
            elif l[0] == "txna":
                transpiled_out += "\ttxna_field(s, TxnGroup[currentTxn], %s, %s);\n" % (l[1], l[2])
            elif l[0] == "gtxns":
                transpiled_out += "\tgtxn_field(s, TxnGroup, %s, %s);\n" % (l[1], l[2])
            elif l[0] == "gtxnsa":
                transpiled_out += "\tgtxna_field(s, TxnGroup, %s, %s);\n" % (l[1], l[2])
            elif l[0] == "extract":
                transpiled_out += "\textract(s, %s, %s);\n" % (l[1], l[2])
            elif l[0] == "substring":
                transpiled_out += "\tsubstring(s, %s, %s);\n" % (l[1], l[2])
            else:
                transpiled_out += unsupported_opcode(l)
        else:
            transpiled_out += unsupported_opcode(l)

    return transpiled_out


def write_fuzzAVM(contract_paths:[str]):
    with open('FuzzAVM.generated.cpp', 'w') as f:
        with open('AVMFuzz/FuzzAVM_template.cpp', 'r') as template:
            print(template.read(), file=f)
            # for l in template:
            #     print(l, file=f)

        for i, path in enumerate(contract_paths):
            parsed_contract = parse_contract(path)
            out = transpile_contract(parsed_contract)

            print("inline void contract_%d(Stack& s, BlockchainState& BS, EvalContext& ctx, vector<Txn>& TxnGroup, uint8_t currentTxn){\n" % i, file=f)
            print(out, file=f)
            print("}", file=f)


write_fuzzAVM(["TEALexample.teal"])