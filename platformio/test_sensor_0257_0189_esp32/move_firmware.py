import os, json, shutil

class Move:
    def __init__(self) -> None:
        self.build_dir = ".pio/build/esp32doit-devkit-v1"
        self.output_firmware = "firmware.bin"
        self.output_spiffs = "spiffs.bin"
        self.dest_path = "../../firmware"
        self.child_dir = "esp32doit-devkit-v1"

    def __move_file__(self, src: str, dest: str):
        try:
            if os.path.exists(dest):
                os.remove(dest)
                print(f"Remove existing file: {dest}")
            shutil.copy(src, dest)
            print(f"Copied {src} to {dest}")
        except Exception as e:
            print(f"Error moving file: {e}")

    def process_file(self, new_firmware_name: str, new_spiffs_name: str=None):
        firmware_src = os.path.join(self.build_dir, self.output_firmware)
        spiffs_src= os.path.join(self.build_dir, self.output_spiffs)
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

        if new_firmware_name and os.path.exists(spiffs_src):
            spiffs_dest = os.path.exists(spiffs_src)
            self.__move_file__(spiffs_src, spiffs_dest)
        elif new_spiffs_name:
            print(f"SPIFFS file not found: {spiffs_src}")

def main():
    # Open and read the configuration file
    with open('build.config.json', 'r') as f:
        config = json.load(f)

    data_name = config["name"]
    data_version = config["version"]
    data_revision = config["revision"]
    typefile_1 = config["typefile-1"]
    typefile_2 = config["typefile-2"]

    out_file_firmware = f"{data_name}-v{data_version}-{data_revision}-{typefile_1}.bin"
    out_file_spiffs = f"{data_name}-v{data_version}-{data_revision}-{typefile_2}.bin"

    # Process files
    move_file = Move();
    move_file.process_file(out_file_firmware, out_file_spiffs)

main()
