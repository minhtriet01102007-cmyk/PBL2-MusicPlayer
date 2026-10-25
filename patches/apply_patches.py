import re, shutil, sys
from pathlib import Path

root = Path.cwd()
SRC = root / 'src'
DLL = root / 'dsa' / 'dlinkedlist' / 'dlinkedlist.h'

def backup(p):
    b = p.with_suffix(p.suffix + '.bak')
    if not b.exists(): shutil.copy(p, b)

pat = re.compile(r'^(?!inline\b|template|class|struct|#|//|\}|namespace|using)\S.*::.*\(')
for name in ['song','user','admin','listener','playlist','album','artist']:
    p = SRC / f'{name}.h'
    if not p.exists():
        print('Khong thay', p); continue
    lines = p.read_text(encoding='utf-8').split('\n')
    n = 0
    for i, l in enumerate(lines):
        if pat.match(l):
            lines[i] = 'inline ' + l; n += 1
    if n:
        backup(p); p.write_text('\n'.join(lines), encoding='utf-8')
    print(f'{p.name}: them inline cho {n} ham')
    
if DLL.exists():
    s = DLL.read_text(encoding='utf-8')
    if 'DoublyLinkedList(const DoublyLinkedList&' in s:
        print('dlinkedlist.h: da co copy constructor')
    else:
        backup(DLL)
        s = s.replace('        ~DoublyLinkedList();\n',
                      '        ~DoublyLinkedList();\n'
                      '        DoublyLinkedList(const DoublyLinkedList& other); // sao chep sau\n'
                      '        DoublyLinkedList& operator=(const DoublyLinkedList& other);\n', 1)
        s = s.rstrip('\n') + '''

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList& other) : head(nullptr), tail(nullptr), current(nullptr), size(0){
        for (Node<T>* n = other.head; n != nullptr; n = n->next){
            this->pushBack(n->data);
            if (n == other.current) this->current = this->tail; // giu nguyen vi tri dang phat
        }
}

template <typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(const DoublyLinkedList& other){
        if (this != &other){
            this->clear();
            for (Node<T>* n = other.head; n != nullptr; n = n->next){
                this->pushBack(n->data);
                if (n == other.current) this->current = this->tail;
            }
        }
        return *this;
}
'''
        DLL.write_text(s, encoding='utf-8')
        print('dlinkedlist.h: da them copy constructor + operator=')
else:
    print('Khong thay', DLL)
