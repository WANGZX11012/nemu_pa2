#include <am.h>
#include <klib.h>
#include <klib-macros.h>

#define FPS     30
#define CPS     5
#define CHAR_W  8
#define CHAR_H  16
#define TARGET_LEN 10
#define RAND_CHARS_LEN 32
#define OFFSET_H 28
#define CHAR_HW (8*16)
#define COL_WHITE    0xeeeeee
#define COL_RED      0xff0033
#define COL_GREEN    0x00cc33
#define COL_PURPLE   0x2a0a40
#define TIME_S 1000000
int vga_h,  vga_w;
uint32_t texture[3][26][CHAR_W * CHAR_H];
extern char font[];
enum { WHITE = 0, RED, GREEN, PURPLE,yellow};
enum {up=1,down,left,right};
int snake_dir = 0;
int clean_pos = 0;
int finish = 0;
uint32_t green_line  [400];
uint32_t purple_line [400];
typedef struct{
    char ch;
    int x,y;
    uint32_t *ptr;
} Player;  
Player snake[2];

typedef struct{
    char ch;
    int x,y;
    uint32_t *data_p;
    int64_t live_time;
} RandomChar;

extern uint8_t vga_load[];

RandomChar rand_chars[RAND_CHARS_LEN];
RandomChar target_str[TARGET_LEN];

uint32_t purple_char [CHAR_HW];
char lut[256]={
    [AM_KEY_UP] = up, [AM_KEY_DOWN] = down, [AM_KEY_LEFT] = left, [AM_KEY_RIGHT] = right,
};

int randint (int l, int r){
    return l + (rand() & 0x7fffffff) % (r - l +1);
}
// 随机坐标
int random_xy(char ch){
    switch (ch){
        case 'x': {
            int col = rand() % 50; // 0~49
            return col * CHAR_W;
        }
        case 'y': {
            int row = rand() % 17; // 0~16
            return row * CHAR_H +OFFSET_H;
        }
        default:
            break;
    }
    return 0;
}

void chars_init(){
    for (int ch = 0; ch < 26; ch++)
    {
        char *c = &font[ch * CHAR_H];
        for (int i = 0,y = 0; y < CHAR_H; y++)
        {
            for (int x = 0; x < CHAR_W; x++,i++)
            {
                int t = (c[y] >> (CHAR_W - x -1)) & 1;
                texture[WHITE][ch][i] = t ? COL_WHITE : COL_PURPLE;
                texture[RED  ][ch][i] = t ? COL_RED   : COL_PURPLE;
                texture[GREEN][ch][i] = t ? COL_GREEN : COL_PURPLE;
            }
            
        }
        
    }
    
}

void succssful(RandomChar *target){
    const char *str = "SUCCESSFUL";
    for (int i = 0; i < TARGET_LEN; i++)
    {
        target[i].ch = str[i];
        target[i].data_p = texture[RED][str[i] - 'A'];
        target[i].live_time = -1;
        io_write(AM_GPU_FBDRAW,target[i].x,target[i].y,target[i].data_p,CHAR_W,CHAR_H,false);
    }
    uint32_t *rgb_ptr = (uint32_t*)vga_load;
    io_write(AM_GPU_FBDRAW, 0, OFFSET_H, rgb_ptr, 400, 272,true);
    while (1) {  
        AM_INPUT_KEYBRD_T key = io_read(AM_INPUT_KEYBRD);
        if (key.keycode == AM_KEY_ESCAPE) halt(0);
    }
}

// 检查新生成的坐标是否与其他字符或蛇重合
bool is_overlap(int x, int y, RandomChar *c) {
    for (int i = 0; i < 2; i++) {
        if (snake[i].x == x && snake[i].y == y) return true;
    }
    for (int i = 0; i < RAND_CHARS_LEN; i++) {
        if (&rand_chars[i] != c && rand_chars[i].live_time > 0 && rand_chars[i].x == x && rand_chars[i].y == y) {
            return true;
        }
    }
    return false;
}

void new_char(RandomChar *array,int len, bool is_rand) {   // 显式传入长度
    for (int i = 0; i < len; i++) {
        RandomChar *c = &array[i];            // 或直接用 array + i
        c->ch = 'A' + randint(0, 25);
        int new_x, new_y;
        do {
            new_x = random_xy('x');
            new_y = random_xy('y');
        } while (is_overlap(new_x, new_y, c)); // 如果重合，重新生成
        c->x = new_x;
        c->y = new_y;
        if (is_rand){
            int cor = (rand() % 2) + 1 ;
            c->data_p = texture[cor][c->ch- 'A'];
        } else{
            c->data_p = texture[WHITE][c->ch- 'A'];
        }
        // 注意 texture 索引类型
        c->live_time = 150; // 这里只是一个初始值，但在 main 里也会改
    }
}

void head_tail_init(Player *snake,RandomChar *target_str,int n){
    
    for (size_t i = 0; i < n; i++)
    {
        Player *s = &snake[i];
        RandomChar *tar = &target_str[i];
        s->ch  = tar->ch;
        s->ptr = tar->data_p;
        if (i==0){
            int pos_x = random_xy('x');
            int pos_y = random_xy('y');
            s->x =  (pos_x == 0  ) ? pos_x + CHAR_W :
                    (pos_x == 392 ) ? pos_x - CHAR_W : pos_x;
            s->y =  (pos_y == OFFSET_H  ) ? pos_y + CHAR_H :
                    (pos_y ==  256) ? pos_y - CHAR_H : pos_y;  
        } else{
            snake_dir = rand() % 4 +1;
            switch (snake_dir){
                case up:
                    s->x = snake[0].x;
                    s->y = snake[0].y - CHAR_H;
                    break;
                case down:
                    s->x = snake[0].x;
                    s->y = snake[0].y + CHAR_H;
                    break;
                case left:
                    s->x = snake[0].x - CHAR_W;
                    s->y = snake[0].y;
                    break;
                case right:
                    s->x = snake[0].x + CHAR_W;
                    s->y = snake[0].y;
                    break;
                default:
                    break;
            }
        }
        io_write(AM_GPU_FBDRAW,snake[i].x,snake[i].y,snake[i].ptr,CHAR_W,CHAR_H,false);
    }
}

void col_line( uint32_t col,uint32_t *addr){
    for (int i = 0; i < vga_w; i++){
        addr[i] = col;
    }
}

void vga_init(){
    bool vga_has = io_read(AM_GPU_CONFIG).present;
    panic_on(!vga_has,"VGA DEVICE DONOT START!");
    vga_h = io_read(AM_GPU_CONFIG).height;
    vga_w = io_read(AM_GPU_CONFIG).width;

    for (int i = 0; i < RAND_CHARS_LEN; i++)
    {
        rand_chars[i].live_time = 0;
    }

    for (int i = 0; i < CHAR_HW; i++)
    {
        purple_char[i] = COL_PURPLE;
    }
    col_line(COL_GREEN,green_line);
    col_line(COL_PURPLE,purple_line);
    for (int i = 0; i < OFFSET_H; i++){
        io_write(AM_GPU_FBDRAW,0,i,green_line,vga_w,1,false);
    }
    for (int i = OFFSET_H; i < vga_h; i++){
        io_write(AM_GPU_FBDRAW, 0, i, purple_line, vga_w, 1, false);
    }
    chars_init();
    int offset_y = (OFFSET_H - CHAR_H) / 2;
    int offset_x = (vga_w - (CHAR_W * TARGET_LEN)) / 2;
    new_char(target_str,TARGET_LEN,true);

    for (int i = 0; i < TARGET_LEN; i++){
        target_str[i].x = offset_x + i * CHAR_W;
        target_str[i].y = offset_y;
        io_write(AM_GPU_FBDRAW, target_str[i].x, target_str[i].y, target_str[i].data_p, CHAR_W, CHAR_H, false);

    }

    head_tail_init(snake,target_str,2);

    for (int i = 0; i < 5; i++) {
        new_char(&rand_chars[i], 1,false);
        io_write(AM_GPU_FBDRAW, rand_chars[i].x, rand_chars[i].y, rand_chars[i].data_p, CHAR_W, CHAR_H, false);
    }
    io_write(AM_GPU_FBDRAW, 0, 0, NULL, 0, 0, true);
}

void render_target() {
    Player *s = &snake[0];
    for (int i = 0; i < TARGET_LEN; i++) {
        if (s->ch == target_str[i].ch && target_str[i].live_time != 0) {
            io_write(AM_GPU_FBDRAW, target_str[i].x, target_str[i].y,
                     purple_char, CHAR_W, CHAR_H, false);
            target_str[i].live_time = 0;
            break;
        }
    }

}

void check_hit(int x, int y) {
    for (int i = 0; i < RAND_CHARS_LEN; i++) {
        if (snake[0].ch == rand_chars[i].ch &&
            x == rand_chars[i].x &&
            y == rand_chars[i].y &&
            rand_chars[i].live_time > 0)
        {
            render_target();  
            rand_chars[i].live_time = 0;
            if (clean_pos > 7){
                Player old_tail = snake[1];
                int prev_head_x = snake[0].x;
                int prev_head_y = snake[0].y;
                snake[0].ch  = old_tail.ch;
                snake[0].ptr = old_tail.ptr;
                snake[0].x   = x;
                snake[0].y   = y;
                snake[1].x   = prev_head_x;
                snake[1].y   = prev_head_y; 
                snake[1].ptr = purple_char;
                if (clean_pos == TARGET_LEN -1)
                {
                    succssful(target_str);
                }
                clean_pos++;
                return; 
            }else{
                // 蛇尾变蛇头
                Player old_tail = snake[1];
                int prev_head_x = snake[0].x;
                int prev_head_y = snake[0].y;
                snake[0].ch  = old_tail.ch;
                snake[0].ptr = old_tail.ptr;
                snake[0].x   = x;
                snake[0].y   = y;
                // 蛇头变蛇尾
                snake[1].ch  = target_str[clean_pos+2].ch;
                snake[1].ptr = target_str[clean_pos+2].data_p;
                snake[1].x   = prev_head_x;  
                snake[1].y   = prev_head_y;
                clean_pos++;
                return;    
            }
        }
    }
    // 普通移动（未吃到）
    snake[1].x = snake[0].x;
    snake[1].y = snake[0].y;
    snake[0].x = x;
    snake[0].y = y;
}

void move(int dir){
    int com_x, com_y;
    switch (dir){
    case up:
        com_x = snake[0].x ;
        com_y = snake[0].y - CHAR_H;    // vga图形坐标  向下Y轴递增
        if (com_y < OFFSET_H ) halt(0);
        check_hit(com_x,com_y);
        break;
    case down:
        com_x = snake[0].x ;
        com_y = snake[0].y + CHAR_H;
        if (com_y  > 292 ) halt(0);
        check_hit(com_x,com_y);
        break;
    case left:
        com_x = snake[0].x - CHAR_W;
        com_y = snake[0].y ;
        if (com_x < 0 ) halt(0);
        check_hit(com_x,com_y);
        break;
    case right:
        com_x = snake[0].x + CHAR_W;
        com_y = snake[0].y ;
        if (com_x > 392) halt(0);
        check_hit(com_x,com_y);
        break;
    default:
        break;
    }
}


int main(){
    ioe_init(); 
    srand(io_read(AM_TIMER_UPTIME).us);
    vga_init();

    bool tim_has = io_read(AM_TIMER_CONFIG).present;
    bool key_has = io_read(AM_INPUT_CONFIG).present;

    panic_on(!tim_has,"TIM DEVICE DONOT START!");
    panic_on(!key_has,"KEY DEVICE DONOT START!");

    uint64_t t0 = io_read(AM_TIMER_UPTIME).us;
    int rendered = 0;
    int current = 0;

    static int old_x[RAND_CHARS_LEN], old_y[RAND_CHARS_LEN], n = 0;
    while (1){
        int target_frame = (io_read(AM_TIMER_UPTIME).us - t0) / (TIME_S / FPS);
        while (current < target_frame) {
            int spawn_count = 0;
            for (int i = 0; i < RAND_CHARS_LEN; i++) {
                if (rand_chars[i].live_time == 0) {
                    new_char(&rand_chars[i], 1,false);
                    // 时间改成活若干『帧』。以 30 FPS 来说，存活5秒就是 150 帧
                    rand_chars[i].live_time = 150; 
                    spawn_count++;
                    if (spawn_count >= 1) break; // 每帧只生成 1 个源源不断，或者改5瞬间刷屏
                }
            }
            for (int i = 0; i < RAND_CHARS_LEN; i++) {
                if (rand_chars[i].live_time > 0) {
                    rand_chars[i].live_time--; 
                }
            }
            current++;
        }

        // --- 键盘逻辑部分 ---
        while (1){
            AM_INPUT_KEYBRD_T  key = io_read(AM_INPUT_KEYBRD);
            if (key.keycode == AM_KEY_NONE) break;
            if (key.keydown && key.keycode == AM_KEY_ESCAPE) halt(0);
            if (key.keydown && lut[key.keycode]){

                move(lut[key.keycode]);
            }
        }

        if (current > rendered) {
            for (int i = 0; i < n; i++) {
                io_write(AM_GPU_FBDRAW, old_x[i], old_y[i], purple_char, CHAR_W, CHAR_H, false);
            }
            

            static int last_head_x = -1, last_head_y = -1;
            static int last_tail_x = -1, last_tail_y = -1;
            if (last_head_x != -1) {
                io_write(AM_GPU_FBDRAW, last_head_x, last_head_y, purple_char, CHAR_W, CHAR_H, false);
                io_write(AM_GPU_FBDRAW, last_tail_x, last_tail_y, purple_char, CHAR_W, CHAR_H, false);
            }

            n = 0;
            for (int i = 0; i < RAND_CHARS_LEN; i++) {
                if (rand_chars[i].live_time > 0) {
                    // 把当前的留档到 old 数组，备着给未来下一帧擦除用！
                    old_x[n] = rand_chars[i].x;
                    old_y[n] = rand_chars[i].y;
                    n++;
                    io_write(AM_GPU_FBDRAW, rand_chars[i].x, rand_chars[i].y, rand_chars[i].data_p, CHAR_W, CHAR_H, false);
                }
            }
            
            // 记录下这帧蛇的位置，用于下一帧擦除
            last_head_x = snake[0].x; last_head_y = snake[0].y;
            if(snake[1].ch != '\0'){
                last_tail_x = snake[1].x; last_tail_y = snake[1].y;
            } else {
                last_tail_x = -1; last_tail_y = -1;
            }

            // 确保蛇永远在最上层，防止被刚吃掉的随机字符背景擦除
            io_write(AM_GPU_FBDRAW, snake[0].x, snake[0].y, snake[0].ptr, CHAR_W, CHAR_H, false);
            if (snake[1].ptr) {
                io_write(AM_GPU_FBDRAW, snake[1].x, snake[1].y, snake[1].ptr, CHAR_W, CHAR_H, false);
            }
            
            // 统一全屏刷新：每一帧（30FPS）只执行 1 次！
            io_write(AM_GPU_FBDRAW, 0, 0, NULL, 0, 0, true);
            rendered = current;
        }
    }
    
}


// ffmpeg -i success.png -vf scale=400:272 -pix_fmt bgra -f rawvideo success.rgb
