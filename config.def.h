/* See LICENSE file for copyright and license details. */

/* appearance */
#include <X11/X.h>
static const unsigned int borderpx = 1;    /* 窗口边框大小 */
static const unsigned int snap     = 32;   /* 边缘依附宽度 */
static const int showbar           = 1;    /* 是否显示状态栏 */
static const int topbar            = 1;    /* 指定状态栏位置: 0 底部, 1 顶部 */
static int gappi                   = 12;   /* 窗口与窗口之间的间隙*/
static int gappo                   = 12;   /* 窗口与边缘之间的间隙*/
static unsigned int baralpha       = 0xc0; /* 状态栏透明度 */
static unsigned int borderalpha    = 0xdd; /* 窗口边框透明度 */
static const char *fonts[]         = {"JetBrainsMono Nerd Font:style=medium:size=13", "monospace:size=10"};
static const char dmenufont[]      = "monospace:size=10";
static const char col_gray1[]      = "#222222";
static const char col_gray2[]      = "#444444";
static const char col_gray3[]      = "#bbbbbb";
static const char col_gray4[]      = "#eeeeee";
static const char col_cyan[]       = "#005577";
static const char *colors[][3]     = {
  /*               fg         bg         border   */
    [SchemeNorm] = {col_gray3, col_gray1, col_gray2},
    [SchemeSel]  = {col_gray4, col_cyan,  col_cyan },
};

/* 自定义脚本 */
static const char *autostartscript = "$DWM/autostart.sh";

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

static const Rule rules[] = {
  /* xprop(1):
  *	WM_CLASS(STRING) = instance, class
  *	WM_NAME(STRING) = title
  */
  /* class      instance    title       tags mask     isfloating   monitor */
    {"Gimp",    NULL, NULL, 0,      1, -1},
    {"Firefox", NULL, NULL, 1 << 8, 0, -1},
};

/* layout(s) */
static const float mfact        = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster        = 1;    /* number of clients in master area */
static const int resizehints    = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1;    /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
  /* symbol     arrange function */
    {"[]=", tile   }, /* first entry is default */
    {"><>", NULL   }, /* no layout function means floating behavior */
    {"[M]", monocle},
};

/* key definitions: super */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                                                                                                                                              \
  {MODKEY, KEY, view, {.ui = 1 << TAG}}, {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}}, {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                                                  \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                                                                                                                                                     \
  {                                                                                                                                                                                                    \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                                                                                                                                               \
  }

static const Key keys[] = {
  /* modifier            key        function        argument          */
    {MODKEY,             XK_t,      togglebar,      {0}               }, /* super + t              | 显示/隐藏状态栏 */

    {MODKEY,             XK_Tab,    focusstack,     {.i = +1}         }, /* super + tab            |  本 tag 内切换聚焦窗口 */
    {MODKEY | ShiftMask, XK_Tab,    focusstack,     {.i = -1}         }, /* super + shift + tab    |  本 tag 内切换聚焦窗口 */
    {MODKEY,             XK_Up,     focusstack,     {.i = -1}         }, /* super + up             |  本 tag 内切换聚焦窗口 */
    {MODKEY,             XK_Down,   focusstack,     {.i = +1}         }, /* super + down           |  本 tag 内切换聚焦窗口 */

    {MODKEY,             XK_i,      incnmaster,     {.i = +1}         },
    {MODKEY,             XK_d,      incnmaster,     {.i = -1}         },

    {MODKEY,             XK_comma,  setmfact,       {.f = -0.05}      }, /* super + ,              | 降低窗口宽度 */
    {MODKEY,             XK_period, setmfact,       {.f = +0.05}      }, /* super + .              | 增加窗口宽度 */

    {MODKEY | ShiftMask, XK_Return, zoom,           {0}               }, /* super + shift + return | 将当前聚焦窗口置为主窗口 */
    {MODKEY,             XK_Tab,    view,           {0}               },
    {MODKEY,             XK_q,      killclient,     {0}               }, /* super + q              | 关闭当前窗口 */
    {MODKEY,             XK_t,      setlayout,      {.v = &layouts[0]}},
    {MODKEY,             XK_f,      setlayout,      {.v = &layouts[1]}},
    {MODKEY,             XK_m,      setlayout,      {.v = &layouts[2]}},
    {MODKEY,             XK_space,  setlayout,      {0}               },
    {MODKEY | ShiftMask, XK_space,  togglefloating, {0}               },
    {MODKEY,             XK_0,      view,           {.ui = ~0}        },
    {MODKEY | ShiftMask, XK_0,      tag,            {.ui = ~0}        },
    {MODKEY,             XK_b,      focusmon,       {.i = -1}         }, /* super + b              | 光标移动到另一个显示器 */
    {MODKEY | ShiftMask, XK_b,      focusmon,       {.i = +1}         }, /* super + shift + b      | 聚焦窗口移动到另一个显示器 */
    {MODKEY | ShiftMask, XK_comma,  tagmon,         {.i = -1}         },
    {MODKEY | ShiftMask, XK_period, tagmon,         {.i = +1}         },
    {MODKEY,             XK_F12,    quit,           {0}               }, /* 退出 dwm */

    {MODKEY,             XK_Return, spawn,          SHCMD("alacritty")}, /* super + return         | 打开终端 */

    TAGKEYS(XK_1, 0)  /* 切换到 tag 1 */
    TAGKEYS(XK_2, 1)  /* 切换到 tag 2 */
    TAGKEYS(XK_3, 2)  /* 切换到 tag 3 */
    TAGKEYS(XK_4, 3)  /* 切换到 tag 4 */
    TAGKEYS(XK_5, 4)  /* 切换到 tag 5 */
    TAGKEYS(XK_6, 5)  /* 切换到 tag 6 */
    TAGKEYS(XK_7, 6)  /* 切换到 tag 7 */
    TAGKEYS(XK_8, 7)  /* 切换到 tag 8 */
    TAGKEYS(XK_9, 8)  /* 切换到 tag 9 */
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
  /* click                event mask      button          function argument */
    {ClkLtSymbol,   0,      Button1, setlayout,      {0}               },
    {ClkLtSymbol,   0,      Button3, setlayout,      {.v = &layouts[2]}},
    {ClkWinTitle,   0,      Button2, zoom,           {0}               },
    {ClkStatusText, 0,      Button2, spawn,          {0}               },
    {ClkClientWin,  MODKEY, Button1, movemouse,      {0}               },
    {ClkClientWin,  MODKEY, Button2, togglefloating, {0}               },
    {ClkClientWin,  MODKEY, Button3, resizemouse,    {0}               },
    {ClkTagBar,     0,      Button1, view,           {0}               },
    {ClkTagBar,     0,      Button3, toggleview,     {0}               },
    {ClkTagBar,     MODKEY, Button1, tag,            {0}               },
    {ClkTagBar,     MODKEY, Button3, toggletag,      {0}               },
};
