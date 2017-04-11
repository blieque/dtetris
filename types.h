typedef enum {
    false = 0,
    true = 1
} bool;

typedef struct point Point;
struct point {
    int x;
    int y;
};

typedef struct game_data GameData;
struct game_data {
    bool keep_running;
    float frame_rate;
    float frame_interval;
    int width;
    int height;
    char** board;
    char* _board_data;
};
