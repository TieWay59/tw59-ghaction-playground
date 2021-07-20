import os
import os.path as path

dir = '.\\python\\test'
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


'''
建议最深的文件，深度不要超过三个文件夹：

- 根目录 # 一级标题不会被转化
    - 第一层文件夹 ## 对应 chapter
        - 第二层文件夹 ### 对应 section
            - 第三层文件夹 #### 对应 subsection
                - 第四层的文件 ##### 对应 subsubsection 

*在第四层之后的路径，都会对应使用五级标题*
'''
if __name__ == '__main__':

    with open('.\\python\\text.md', 'w+', encoding='utf8') as f:
        for root, dirs, files in os.walk(dir):
            root_list = root[len(dir):].split('\\')
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
