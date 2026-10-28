import socket
import time


def checksum(text):
    total = 0
    for ch in text:
        total = total + ord(ch)
    return total % 256


def connect(port, wait_seconds):
    deadline = time.time() + wait_seconds
    while time.time() < deadline:
        try:
            sock = socket.create_connection(("127.0.0.1", port), timeout=5)
            sock.settimeout(30)
            sock.sendall(b"+")
            return sock
        except OSError:
            time.sleep(0.5)
    return None


def read_packet(sock):
    data = ""
    started = False
    while True:
        ch = sock.recv(1).decode("latin-1")
        if ch == "":
            return None
        if not started:
            if ch == "$":
                started = True
            continue
        if ch == "#":
            sock.recv(2)
            sock.sendall(b"+")
            return data
        data = data + ch


def send_packet(sock, text):
    packet = "$" + text + "#" + format(checksum(text), "02x")
    sock.sendall(packet.encode("latin-1"))
    while True:
        ack = sock.recv(1).decode("latin-1")
        if ack == "+":
            return
        if ack == "-":
            sock.sendall(packet.encode("latin-1"))
        if ack == "":
            return


def command(sock, text):
    send_packet(sock, text)
    return read_packet(sock)


def interrupt(sock):
    sock.sendall(b"\x03")
    return read_packet(sock)


def read_registers(sock):
    reply = command(sock, "g")
    registers = []
    if reply is None or reply.startswith("E"):
        return registers
    i = 0
    while i < 17 and (i + 1) * 8 <= len(reply):
        chunk = reply[i * 8:(i + 1) * 8]
        value = int.from_bytes(bytes.fromhex(chunk), "little")
        registers.append(value)
        i = i + 1
    return registers


def write_memory(sock, address, data):
    reply = command(sock, "M" + format(address, "x") + "," + format(len(data), "x") + ":" + data.hex())
    return reply == "OK"


def add_breakpoint(sock, address, thumb):
    kind = 4
    if thumb:
        kind = 2
    reply = command(sock, "Z0," + format(address, "x") + "," + str(kind))
    return reply == "OK"


def remove_breakpoint(sock, address, thumb):
    kind = 4
    if thumb:
        kind = 2
    reply = command(sock, "z0," + format(address, "x") + "," + str(kind))
    return reply == "OK"


def wait_for_stop(sock, seconds):
    previous_timeout = sock.gettimeout()
    sock.settimeout(seconds)
    try:
        packet = read_packet(sock)
    except socket.timeout:
        packet = None
    finally:
        sock.settimeout(previous_timeout)
    return packet


def read_memory(sock, address, length):
    result = bytearray()
    while len(result) < length:
        size = length - len(result)
        if size > 0x100:
            size = 0x100
        reply = command(sock, "m" + format(address + len(result), "x") + "," + format(size, "x"))
        if reply is None or reply.startswith("E") or reply == "":
            return None
        result = result + bytes.fromhex(reply)
    return bytes(result)
