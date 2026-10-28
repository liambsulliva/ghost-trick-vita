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

public class DecompileFunctions extends GhidraScript {

    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        String outPath = args[0];
        DecompInterface decompiler = new DecompInterface();
        decompiler.openProgram(currentProgram);
        PrintWriter out = new PrintWriter(outPath);
        for (int i = 1; i < args.length; i++) {
            String text = args[i];
            boolean thumb = false;
            boolean hasMode = false;
            if (text.startsWith("thumb:")) {
                hasMode = true;
                thumb = true;
                text = text.substring(6);
            }
            if (text.startsWith("arm:")) {
                hasMode = true;
                text = text.substring(4);
            }
            Address address = currentProgram.getAddressFactory().getAddress(text);
            Function function = getFunctionAt(address);
            if (function == null && hasMode) {
                Register tmode = currentProgram.getProgramContext().getRegister("TMode");
                BigInteger mode = BigInteger.ZERO;
                if (thumb) {
                    mode = BigInteger.ONE;
                }
                DisassembleCommand command = new DisassembleCommand(address, null, true);
                command.setInitialContext(new RegisterValue(tmode, mode));
                command.applyTo(currentProgram, monitor);
                function = createFunction(address, null);
            }
            if (function == null) {
                out.println("// no function at " + args[i]);
                continue;
            }
            DecompileResults results = decompiler.decompileFunction(function, 60, monitor);
            out.println("// " + function.getName() + " at " + args[i]);
            if (results.decompileCompleted()) {
                out.println(results.getDecompiledFunction().getC());
            } else {
                out.println("// decompile failed: " + results.getErrorMessage());
            }
        }
        out.close();
        decompiler.dispose();
        println("wrote " + outPath);
    }
}
