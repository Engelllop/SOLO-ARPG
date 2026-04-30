content = open(r'E:\SOLO\Design\Quests\n2_body.txt', 'r', encoding='utf-8').read()
with open(r'E:\SOLO\Design\Quests\N2_pastor_cenizas.md', 'w', encoding='utf-8') as f:
    f.write(content)
print(f'Copied: {len(content)} bytes')
