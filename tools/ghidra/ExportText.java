import java.io.PrintWriter;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.Collections;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressRange;
import ghidra.program.model.lang.Register;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.mem.MemoryBlock;

public class ExportText extends GhidraScript {

    @Override
    public void run() throws Exception {
        String outPath = getScriptArgs()[0];
        ArrayList<String> blockLines = new ArrayList<String>();
        MemoryBlock[] blocks = currentProgram.getMemory().getBlocks();
        for (int i = 0; i < blocks.length; i++) {
            MemoryBlock block = blocks[i];
            String hash = "-";
            if (block.isInitialized()) {
                byte[] bytes = new byte[(int) block.getSize()];
                block.getBytes(block.getStart(), bytes);
                hash = sha1(bytes);
            }
            blockLines.add("block " + block.getName() + " " + block.getStart().toString(true) + " " + block.getSize() + " " + hash);
        }
        Collections.sort(blockLines);

        Register tmode = currentProgram.getProgramContext().getRegister("TMode");
        ArrayList<String> functionLines = new ArrayList<String>();
        FunctionIterator functions = currentProgram.getFunctionManager().getFunctions(true);
        while (functions.hasNext()) {
            Function function = functions.next();
            Address entry = function.getEntryPoint();
            String mode = "arm";
            BigInteger value = currentProgram.getProgramContext().getValue(tmode, entry, false);
            if (value != null && value.intValue() == 1) {
                mode = "thumb";
            }
            String ranges = "";
            int rangeCount = 0;
            for (AddressRange range : function.getBody().getAddressRanges()) {
                ranges = ranges + " " + range.getMinAddress().toString(true) + " " + range.getMaxAddress().toString(true);
                rangeCount = rangeCount + 1;
            }
            functionLines.add("function " + entry.toString(true) + " " + mode + " " + function.getName() + " " + rangeCount + ranges);
        }
        Collections.sort(functionLines);

        PrintWriter out = new PrintWriter(outPath);
        for (String line : blockLines) {
            out.println(line);
        }
        for (String line : functionLines) {
            out.println(line);
        }
        out.close();
        println("exported " + blockLines.size() + " blocks and " + functionLines.size() + " functions to " + outPath);
    }

    private String sha1(byte[] bytes) throws Exception {
        MessageDigest digest = MessageDigest.getInstance("SHA-1");
        byte[] result = digest.digest(bytes);
        String text = "";
        for (int i = 0; i < result.length; i++) {
            text = text + String.format("%02x", result[i] & 0xFF);
        }
        return text;
    }
}
