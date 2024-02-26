import os


def decode(dir, filename: str):
    file = open(os.path.join(dir, filename), "rb")
    bytes = file.read()
    byte_groups = []
    index = 0
    
    while index < len(bytes):
        byte_group = []
        continuation_bit = True
        while continuation_bit:
            # Drop continuation bit from appended value
            byte_group.append(bytes[index] & 0x7F)
            continuation_bit = bytes[index] & 0x80
            index += 1
        byte_groups.append(byte_group.reverse())

    if continuation_bit:
        raise ValueError("Continuation bit set, but reached end of bytestream")

    return byte_groups


def decode_files(dir, filenames: str):
    for file in filenames:
        if file.endswith(".uint64"):
            print(f"File '{file}' has byte values (in decimal):{decode(dir, file)}")

if __name__ == "__main__":
    dir_name = 'varint'
    files = os.listdir(dir_name)
    decode_files(dir_name, files)
    
