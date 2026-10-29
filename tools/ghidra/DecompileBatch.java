import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.PrintWriter;
import java.math.BigInteger;

import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.lang.Register;
import ghidra.program.model.lang.RegisterValue;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.SourceType;

public class DecompileBatch extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        File folder = new File(args[1]);
        folder.mkdirs();
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
            Address address = currentProgram.getAddressFactory().getAddress(parts[2]);
            String status = "no_function";
            try {
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
                    DecompileResults result = decompiler.decompileFunction(function, 10, monitor);
                    if (result.decompileCompleted()) {
                        PrintWriter source = new PrintWriter(new File(folder, parts[0] + ".c"));
                        source.println(result.getDecompiledFunction().getC());
                        source.close();
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
}
