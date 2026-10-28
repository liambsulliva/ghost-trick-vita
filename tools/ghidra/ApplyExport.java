import java.io.BufferedReader;
import java.io.FileReader;
import java.math.BigInteger;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.lang.Register;
import ghidra.program.model.symbol.SourceType;

public class ApplyExport extends GhidraScript {

    @Override
    public void run() throws Exception {
        String exportPath = getScriptArgs()[0];
        Register tmode = currentProgram.getProgramContext().getRegister("TMode");
        BufferedReader reader = new BufferedReader(new FileReader(exportPath));
        int created = 0;
        int failed = 0;
        String line = reader.readLine();
        while (line != null) {
            String[] parts = line.trim().split(" ");
            if (parts[0].equals("function")) {
                Address entry = readAddress(parts[1]);
                String mode = parts[2];
                String name = parts[3];
                int rangeCount = Integer.parseInt(parts[4]);
                AddressSet body = new AddressSet();
                for (int i = 0; i < rangeCount; i++) {
                    Address start = readAddress(parts[5 + i * 2]);
                    Address end = readAddress(parts[6 + i * 2]);
                    body.addRange(start, end);
                    if (mode.equals("thumb")) {
                        currentProgram.getProgramContext().setValue(tmode, start, end, BigInteger.ONE);
                    }
                }
                try {
                    currentProgram.getFunctionManager().createFunction(name, entry, body, SourceType.USER_DEFINED);
                    created = created + 1;
                } catch (Exception e) {
                    println("could not create " + name + ": " + e.getMessage());
                    failed = failed + 1;
                }
            }
            line = reader.readLine();
        }
        reader.close();
        println("created " + created + " functions, " + failed + " failed");
    }

    private Address readAddress(String text) {
        return currentProgram.getAddressFactory().getAddress(text);
    }
}
