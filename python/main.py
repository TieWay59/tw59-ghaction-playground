import os
import os.path as path

code_dir = '.\\code'
output_dir = '.\\assets\\build.md'
ext_dict = dict(
    {
        'c': 'cpp',
        'cc': 'cpp',
        'c++': 'cpp',
        'cpp': 'cpp',
        'java': 'java',
        'txt': 'text',
        'py': 'python',
    }
)


def wrapper(lines, ext):
    return ["```{}\n".format(ext)] + lines + ["\n```\n\n"]


if __name__ == '__main__':

    with open(output_dir, 'w+', encoding='utf8') as f:
        for root, dirs, files in os.walk(code_dir):
            root_list = root[len(code_dir):].split('\\')
            f.write('#' * min(5, len(root_list)) +
                    ' ' + root_list[-1] + '\n\n')
            for file in files:
                ext = file.split('.')[-1]
                if not ext_dict.__contains__(ext):
                    continue
                with open(path.join(root, file), 'r', encoding='utf8') as code_file:
                    f.write('#' * min(5, len(root_list) + 1) +
                            ' ' + file + '\n\n')
                    f.writelines(wrapper(code_file.readlines(), ext))

    print('process finished')
