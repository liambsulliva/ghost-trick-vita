import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.PrintWriter;
import java.math.BigInteger;
import java.util.Iterator;

import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.cmd.function.DecompilerParameterIdCmd;
import ghidra.app.decompiler.ClangToken;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.data.DataType;
import ghidra.program.model.data.UnsignedIntegerDataType;
import ghidra.program.model.lang.Register;
import ghidra.program.model.lang.RegisterValue;
import ghidra.program.model.listing.Function;
import ghidra.program.model.pcode.HighVariable;
import ghidra.program.model.symbol.SourceType;

public class DecompileBatch extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        File folder = new File(args[1]);
        folder.mkdirs();
        if (args.length > 2 && args[2].equals("params")) {
            BufferedReader prototypes = new BufferedReader(new FileReader(args[0]));
            AddressSet entries = new AddressSet();
            String target;
            while ((target = prototypes.readLine()) != null) {
                if (target.trim().isEmpty()) {
                    continue;
                }
                Function function = prepareFunction(target.split("\t"));
                if (function != null) {
                    function.setSignatureSource(SourceType.DEFAULT);
                    entries.add(function.getEntryPoint());
                }
            }
            prototypes.close();
            DecompilerParameterIdCmd command = new DecompilerParameterIdCmd(
                "Recover parameters", entries, SourceType.USER_DEFINED, true, false, 10);
            if (!command.applyTo(currentProgram, monitor)) {
                throw new Exception(command.getStatusMsg());
            }
            println("recovered parameters for " + entries.getNumAddresses() + " functions");
        }
        if (args.length > 2 && args[2].equals("r0")) {
            BufferedReader prototypes = new BufferedReader(new FileReader(args[0]));
            String target;
            int prepared = 0;
            while ((target = prototypes.readLine()) != null) {
                if (target.trim().isEmpty()) {
                    continue;
                }
                try {
                    Function function = prepareFunction(target.split("\t"));
                    if (function != null) {
                        DataType type = function.getReturnType();
                        if (type.getLength() != 8 && !type.getName().equals("float")) {
                            function.setReturnType(UnsignedIntegerDataType.dataType, SourceType.USER_DEFINED);
                            prepared++;
                        }
                    }
                } catch (Exception error) {
                    println("could not prepare " + target.split("\t")[0]);
                }
            }
            prototypes.close();
            println("prepared r0 returns for " + prepared + " functions");
        }
        DecompInterface decompiler = new DecompInterface();
        decompiler.openProgram(currentProgram);
        BufferedReader input = new BufferedReader(new FileReader(args[0]));
        PrintWriter report = new PrintWriter(new File(folder, "results.tsv"));
        report.println("name\tstatus");
        String line;
        int count = 0;
        while ((line = input.readLine()) != null) {
            if (line.trim().isEmpty()) {
                continue;
            }
            String[] parts = line.split("\t");
            String status = "no_function";
            try {
                Function function = prepareFunction(parts);
                if (function != null) {
                    DecompileResults result = decompiler.decompileFunction(function, 10, monitor);
                    if (result.decompileCompleted()) {
                        PrintWriter source = new PrintWriter(new File(folder, parts[0] + ".c"));
                        source.println(result.getDecompiledFunction().getC());
                        source.close();
                        PrintWriter types = new PrintWriter(new File(folder, parts[0] + ".types.tsv"));
                        Iterator<ClangToken> tokens = result.getCCodeMarkup().tokenIterator(true);
                        while (tokens.hasNext()) {
                            ClangToken token = tokens.next();
                            if (!token.getText().matches("(?:_?DAT_|[a-z]+Ram)(?:ov[0-9]+__)?[0-9a-fA-F]{8}")) {
                                continue;
                            }
                            HighVariable variable = token.getHighVariable();
                            if (variable != null) {
                                types.println(token.getText() + "\t" + variable.getDataType().getDisplayName());
                            }
                        }
                        types.close();
                        status = "decompiled";
                    } else {
                        status = "decompile_failed";
                    }
                }
            } catch (Exception error) {
                status = "error";
            }
            report.println(parts[0] + "\t" + status);
            report.flush();
            count++;
            if (count % 100 == 0) {
                println("processed " + count);
            }
        }
        input.close();
        report.close();
        decompiler.dispose();
        println("processed " + count + " functions");
    }

    private Function prepareFunction(String[] parts) throws Exception {
        Address address = currentProgram.getAddressFactory().getAddress(parts[2]);
        Function function = getFunctionAt(address);
        if (function == null) {
            Register tmode = currentProgram.getProgramContext().getRegister("TMode");
            BigInteger mode = parts[1].equals("thumb") ? BigInteger.ONE : BigInteger.ZERO;
            DisassembleCommand command = new DisassembleCommand(address, null, true);
            command.setInitialContext(new RegisterValue(tmode, mode));
            command.applyTo(currentProgram, monitor);
            function = createFunction(address, null);
        }
        if (function != null) {
            function.setName(parts[0], SourceType.USER_DEFINED);
        }
        return function;
    }
}
