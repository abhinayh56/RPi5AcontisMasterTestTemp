import os

def print_tree(root_path, prefix=""):
    try:
        items = os.listdir(root_path)
    except PermissionError:
        print(prefix + "└── [Permission Denied]")
        return

    items.sort()
    total_items = len(items)

    for index, item in enumerate(items):
        path = os.path.join(root_path, item)
        is_last = index == total_items - 1

        if is_last:
            connector = "└── "
            new_prefix = prefix + "    "
        else:
            connector = "├── "
            new_prefix = prefix + "│   "

        print(prefix + connector + item)

        if os.path.isdir(path):
            print_tree(path, new_prefix)


if __name__ == "__main__":
    root_directory = input("Enter directory path: ").strip()

    if not os.path.exists(root_directory):
        print("Invalid path!")
    else:
        print(root_directory)
        print_tree(root_directory)