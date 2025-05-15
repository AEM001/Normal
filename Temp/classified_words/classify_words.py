import os

def classify_words(input_file):
    # 创建一个字典来存储分类结果
    word_dict = {}
    
    # 读取输入文件
    with open(input_file, 'r', encoding='utf-8') as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
                
            # 分割单词和数字
            parts = line.rsplit(' ', 1)
            if len(parts) != 2:
                continue
                
            word, num = parts
            num = int(num)
            
            # 添加到字典中
            if num not in word_dict:
                word_dict[num] = []
            word_dict[num].append(word)
    
    # 创建输出目录
    output_dir = os.path.join(os.path.dirname(input_file), "classified_words")
    os.makedirs(output_dir, exist_ok=True)
    
    # 写入分类结果
    for num, words in word_dict.items():
        output_file = os.path.join(output_dir, f"{num}.txt")
        with open(output_file, 'w', encoding='utf-8') as f:
            for word in words:
                f.write(f"{word}\n")
    
    print(f"分类完成，结果保存在 {output_dir} 目录下")

# 使用示例
if __name__ == "__main__":
    input_file = "d:/Code/Normal/Temp/1.txt"  # 替换为你的实际文件路径
    classify_words(input_file)
