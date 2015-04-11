#include <pebble.h>

//first define all the constanstants
#define NUM_ITEMS_FIRST = 4; //the number of menu items in the first section (Breakfast,Lunch,Dinner,Snacks)
#define NUM_SECTIONS = 1; //the amount of sections in the menu
#define NUM_ICONS = 0 ; //the number of icons in the menu

//the s_ before the variables shows they are static
static Window *s_menu_window; // the window for the menu
static MenuLayer *s_menu_layer; //the main menu for the app
static GBitMap *s_menu_icons[NUM_ICONS]; //the array of menu icons


//create the callback methods to get information about the menu so we can render it
static int s_current_icon = 0;

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data){
  return NUM_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data){
  switch(section_index){
    case 0:
      return NUM_ITEMS_FIRST;
    default:
      return 0;
  }
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data){
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      // Draw title text in the section header
      menu_cell_basic_header_draw(ctx, cell_layer, "Meal");
      break;
  }
}

