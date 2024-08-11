import os, json, shutil
from SCons.Script import DefaultEnvironment

# env::<board type?>
# example : esp32doit-devkit-v1, nodemcuv2, uno
board     = "esp32doit-devkit-v1"
child     = "esp32doit-devkit-v1"
firmware  = "firmware.bin" # or firmware.hex
filesys   = "littlefs.bin" # or spiffs.bin
extension = ".bin"

class Move:
    def __init__(self) -> None:
        self.build_dir = f".pio/build/{board}"
        self.output_firmware = firmware
        self.output_fs = filesys
        self.dest_path = "../../firmware"
        self.child_dir = child

    def __move_file__(self, src: str, dest: str):
        try:
            if os.path.exists(dest):
                os.remove(dest)
                print(f"Remove existing file: {dest}")
            shutil.copy(src, dest)
            print(f"Copied {src} to {dest}")
        except Exception as e:
            print(f"Error moving file: {e}")

    def process_file(self, new_firmware_name: str, new_fs_name: str=None):
        firmware_src = os.path.join(self.build_dir, self.output_firmware)
        fs_src= os.path.join(self.build_dir, self.output_fs)
        dest_dir = os.path.join(self.dest_path, self.child_dir)

        if not os.path.exists(dest_dir):
            os.makedirs(dest_dir)
            print(f"Create directory: {dest_dir}")
        else:
            print(f"Direcotry already exists: {dest_dir}")

        if os.path.exists(firmware_src):
            firmware_dest = os.path.join(dest_dir, new_firmware_name)
            self.__move_file__(firmware_src, firmware_dest)
        else:
            print(f"Firmware file not found: {firmware_src}")

        if new_firmware_name and os.path.exists(fs_src):
            fs_dest = os.path.join(dest_dir, new_fs_name)
            self.__move_file__(fs_src, fs_dest)
        elif new_fs_name:
            print(f"FS file not found: {fs_src}")

def main(source, target, env):
    # Open and read the configuration file
    with open('build.config.json', 'r') as f:
        config = json.load(f)

    data_name = config["name"]
    data_version = config["version"]
    data_revision = config["revision"]
    typefile_1 = config["typefile-1"]
    typefile_2 = config["typefile-2"]

    out_file_firmware = f"{data_name}-v{data_version}-{data_revision}-{typefile_1}.bin"
    out_file_fs = f"{data_name}-v{data_version}-{data_revision}-{typefile_2}.bin"

    # Process files
    move_file = Move();
    move_file.process_file(out_file_firmware, out_file_fs)

env = DefaultEnvironment()
env.AddPostAction("$BUILD_DIR/${PROGNAME}" + extension, main)
env.AddPostAction(f"$BUILD_DIR/{filesys}", main)
