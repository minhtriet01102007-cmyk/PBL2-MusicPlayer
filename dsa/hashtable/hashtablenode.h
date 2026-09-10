#pragma once

enum NodeState{ 
    EMPTY,      // Ô hoàn toàn trống
    OCCUPIED,   // Ô đang chứa dữ liệu hợp lệ
    DELETED     // Ô đã từng có dữ liệu nhưng bị xóa 
};
struct HashNode{
    int key;
    int value;
    NodeState state;
};
