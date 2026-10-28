import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;

public class LoadNdsMap extends GhidraScript {

    @Override
    public void run() throws Exception {
        String ndsDir = getScriptArgs()[0];
        Memory memory = currentProgram.getMemory();
        MemoryBlock first = memory.getBlock(toAddr(0x02000000L));
        if (first != null) {
            first.setName("main");
            first.setRead(true);
            first.setWrite(true);
            first.setExecute(true);
        }

        BufferedReader reader = new BufferedReader(new FileReader(new File(ndsDir, "loadmap.txt")));
        String line = reader.readLine();
        while (line != null) {
            String[] parts = line.trim().split(" ");
            String kind = parts[0];
            if (kind.equals("entry")) {
                Address entry = toAddr(Long.decode(parts[1]));
                currentProgram.getSymbolTable().addExternalEntryPoint(entry);
                createLabel(entry, "entry", true);
            }
            if (kind.equals("segment") && !parts[1].equals("main")) {
                addFileBlock(parts[1], Long.decode(parts[2]), Long.decode(parts[3]), new File(ndsDir, parts[4]), false);
            }
            if (kind.equals("overlay")) {
                addFileBlock(parts[1], Long.decode(parts[2]), Long.decode(parts[3]), new File(ndsDir, parts[4]), true);
            }
            if (kind.equals("bss")) {
                MemoryBlock block = memory.createUninitializedBlock(parts[1], toAddr(Long.decode(parts[2])), Long.decode(parts[3]), false);
                block.setRead(true);
                block.setWrite(true);
            }
            if (kind.equals("overlay_bss")) {
                MemoryBlock block = memory.createUninitializedBlock(parts[1], toAddr(Long.decode(parts[2])), Long.decode(parts[3]), true);
                block.setRead(true);
                block.setWrite(true);
            }
            line = reader.readLine();
        }
        reader.close();
        println("memory map loaded from " + ndsDir);
    }

    private void addFileBlock(String name, long address, long size, File file, boolean overlay) throws Exception {
        FileInputStream input = new FileInputStream(file);
        MemoryBlock block = currentProgram.getMemory().createInitializedBlock(name, toAddr(address), input, size, monitor, overlay);
        input.close();
        block.setRead(true);
        block.setWrite(true);
        block.setExecute(true);
    }
}
