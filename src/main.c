#include <genesis.h>
#include <resources.h>
#include <sinuses.h>
#include <scrolltext.h>

u16 ind = TILE_USERINDEX;
int logoX = 80;
int logoY = 0;
int j = 0;
int k = 0;
int l = 0;

float hscroll_offset = 0;
float hscroll_offset_fore = 0;

#define SCROLL_SPRITE_WIDTH 16
#define SCROLL_SPRITE_SPACING 4
#define NUM_SCROLL_SPRITES ((320/(SCROLL_SPRITE_WIDTH+SCROLL_SPRITE_SPACING))+1)

#define FONT_ANIM_A 0
#define FONT_ANIM_DOT 26
#define FONT_ANIM_EXCLAMATION 27
#define FONT_ANIM_COMMA 28
#define FONT_ANIM_QUESTION 29
#define FONT_ANIM_DASH 30
#define FONT_ANIM_SPACE 31
//#define FONT_ANIM_0 32  

Sprite* sprScroll[NUM_SCROLL_SPRITES];
Sprite* logo;

char getAnimFromChar( char zeichen )
{
    if( zeichen >= 'A' && zeichen <= 'Z')
    {
        return (zeichen - 'A') + FONT_ANIM_A;
    }
/*  if( zeichen >= '0' && zeichen <= '9')
    {return (zeichen - '0') + FONT_ANIM_0;}*/

    switch( zeichen )
    {
        case '.': return FONT_ANIM_DOT;
        case '!': return FONT_ANIM_EXCLAMATION;
        case ',': return FONT_ANIM_COMMA;
        case '?': return FONT_ANIM_QUESTION;
        case '-': return FONT_ANIM_DASH;
    }
    return FONT_ANIM_SPACE;
}

void initScrollSprites()
{
    for( int i=0; i<NUM_SCROLL_SPRITES; ++i )
    {
        sprScroll[ i ] = SPR_addSprite(&font01, 320, 240, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
        SPR_setAnim(sprScroll[ i ], getAnimFromChar(' '));
    }
}
 
u32 scrollX=0;
 
void setScrollSprites()
{
    s16 y = 194;
    s16 xModulo = scrollX % (SCROLL_SPRITE_WIDTH + SCROLL_SPRITE_SPACING);
    s16 xDivide = scrollX / (SCROLL_SPRITE_WIDTH + SCROLL_SPRITE_SPACING);
    s16 x = - (1 + xModulo);
    s16 scrollIndex = xDivide;

    for( int i=0; i<NUM_SCROLL_SPRITES; ++i )
    {
        const s16 displayY = y + scrollerYSine[((i*(SCROLL_SPRITE_WIDTH + SCROLL_SPRITE_SPACING)-xModulo)) % sizeof( scrollerYSine ) ];
        const char zeichen = scrolltext[ scrollIndex % (sizeof( scrolltext )-1) ];
        const s16 displayX = (zeichen == ' ') ? 320 : x;
        SPR_setPosition( sprScroll[ i ], displayX, displayY );
        SPR_setAnim( sprScroll[ i ], getAnimFromChar( zeichen ) );
        x += SCROLL_SPRITE_WIDTH + SCROLL_SPRITE_SPACING;
        ++scrollIndex;
    }
    scrollX = scrollX + 2;
}
 
void logoMove()
{
    if (j < 255){
        j = j + 1;
    }
    else {
        j = 0;
    }
    if (k < 255){
        k = k + 1;
    }
    else {
        k = 0;
    }
    logoX = logoXSine[j];
    logoY = logoYSine[k];
    SPR_setPosition(logo, 50+logoX, 5+logoY);
}
 
void intro(){

    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);

    SPR_init();

    initScrollSprites();
    
    PAL_setPalette(PAL0, title01.palette->data, DMA);
    PAL_setPalette(PAL1, black.palette->data, DMA);

    VDP_drawImageEx(BG_A, &title01, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
    //ind += title01.tileset->numTile;
	
    XGM_setLoopNumber(-1);
	XGM_startPlay(&track01);

    waitTick(3000);

    VDP_drawImageEx(BG_A, &black, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 0, 0, TRUE, TRUE);
    ind += black.tileset->numTile;
    //VDP_drawImageEx(BG_B, &black, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 0, 0, TRUE, TRUE);

    mainpart();
}

void mainpart(){

    //PAL_setPalette(PAL1, black.palette->data, DMA);
    //VDP_drawImageEx(BG_A, &black, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 0, 0, TRUE, TRUE);
    
    PAL_setPalette(PAL1, space01.palette->data, DMA);
    VDP_drawImageEx(BG_B, &space01, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 0, -4, FALSE, TRUE);
    ind += space01.tileset->numTile;

    PAL_setPalette(PAL0, back10.palette->data, DMA);
    VDP_drawImageEx(BG_B, &back10, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 9, FALSE, TRUE);
    ind += back10.tileset->numTile;

    VDP_drawImageEx(BG_A, &clouds01, TILE_ATTR_FULL(PAL1, TRUE, FALSE, FALSE, ind), -4, 0, FALSE, TRUE);
    ind += clouds01.tileset->numTile;

    //PAL_setPalette(PAL2, clouds01.palette->data, DMA);

    PAL_setPalette(PAL2, font01.palette->data, DMA);

    PAL_setPalette(PAL3, logo01.palette->data, DMA);
    logo = SPR_addSprite(&logo01, 320, 240, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));
    SPR_setPosition(logo, 320,240);

    while(1)
    {
        VDP_setHorizontalScroll(BG_B, hscroll_offset);
        VDP_setHorizontalScroll(BG_A, hscroll_offset_fore);
        hscroll_offset -= 0.5;
        hscroll_offset_fore -=1;

        setScrollSprites();
        logoMove();

        SPR_update();

        SYS_doVBlankProcess();
    }
}

int main()
{
    intro();

    return (0);
}