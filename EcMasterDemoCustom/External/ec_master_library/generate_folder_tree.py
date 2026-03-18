import os

ORDERED_FOLDERS = ["Bin", "Doc", "Examples", "SDK", "Sources"]

def write_filtered_tree(root_path, output_file):
    root_path = os.path.abspath(root_path)
    lines = []

    def build_tree(path, prefix=""):
        try:
            entries = sorted(os.listdir(path))
        except PermissionError:
            return

        entries_count = len(entries)

        for i, entry in enumerate(entries):
            full_path = os.path.join(path, entry)
            is_last = (i == entries_count - 1)

            connector = "└── " if is_last else "├── "
            line = prefix + connector + entry

            if os.path.isdir(full_path):
                lines.append(line + "/")
                new_prefix = prefix + ("    " if is_last else "│   ")
                build_tree(full_path, new_prefix)
            else:
                lines.append(line)

    # Root
    lines.append(os.path.basename(root_path) + "/")

    # Only include selected subfolders in fixed order
    existing = [f for f in ORDERED_FOLDERS if os.path.isdir(os.path.join(root_path, f))]
    total = len(existing)

    for i, folder in enumerate(existing):
        full_path = os.path.join(root_path, folder)
        is_last = (i == total - 1)

        connector = "└── " if is_last else "├── "
        lines.append(connector + folder + "/")

        prefix = "    " if is_last else "│   "
        build_tree(full_path, prefix)

    with open(output_file, "w", encoding="utf-8") as f:
        f.write("\n".join(lines))


if __name__ == "__main__":
    folder_path = r"D:\qnx\acontis\SSInnovations EC Master\EC-Master-V3.2-QNX8-ARM_64Bit-Eval"
    output_file = "folder_tree.txt"
    write_filtered_tree(folder_path, output_file)
    print(f"Saved hierarchy to {output_file}")