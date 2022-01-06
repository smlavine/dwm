/* See LICENSE file for copyright and license details. */

#include "gaplessgrid.c"
#include "fibonacci.c"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_cyan[]        = "#005577";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_red[]         = "#FF0000";
static const char col_white[]       = "#FFFFFF";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_white, col_gray1, col_gray2 },
	[SchemeSel]  = { col_white, col_cyan,  col_red   },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	/*
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
	*/
	NULL
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol  arrange function */
	{ "T",     tile },    /* first entry is default */
	{ "F",     NULL },    /* no layout function means floating behavior */
	{ "M",     monocle },
	{ "G",     gaplessgrid },
	{ "S",     spiral },
	{ "D",     deck },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_Return, spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_space,  spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_Tab,    zoom,           {0} },
	{ MODKEY,                       XK_r,      rotatestack,    {.i = +1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_g,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_s,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,             XK_d,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_comma,  setcfact,       {.f = +0.25} },
	{ MODKEY,                       XK_period, setcfact,       {.f = -0.25} },
	{ MODKEY,                       XK_backslash, setcfact,    {.f =  0.00} },
	{ MODKEY,                       XK_backslash, setmfact,    {.f =  1.50} },
	{ MODKEY,                       XK_backslash, setlayout,   {.v = &layouts[0]} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

	/* program keybindings */
	{ MODKEY,                       XK_d,         spawn,       SHCMD("mycmus") },
	{ MODKEY,                       XK_f,         spawn,       SHCMD("firefox") },
	{ MODKEY|ShiftMask,             XK_h,         spawn,       SHCMD("st -e htop") },
	{ MODKEY|ShiftMask,             XK_j,         spawn,       SHCMD("jdkdocs") },
	{ MODKEY,                       XK_m,         spawn,       SHCMD("st -e cmus") },
	{ MODKEY,                       XK_n,         spawn,       SHCMD("pausemusic") },
	{ MODKEY,                       XK_o,         spawn,       SHCMD("watchyt -c") },
	{ MODKEY|ShiftMask,             XK_o,         spawn,       SHCMD("watchyt -cd") },
	{ MODKEY,                       XK_p,         spawn,       SHCMD("mypassmenu") },
	{ MODKEY|ShiftMask,             XK_p,         spawn,       SHCMD("firefox --private-window") },
	{ MODKEY,                       XK_s,         spawn,       SHCMD("shoot") },
	{ MODKEY,                       XK_t,         spawn,       SHCMD("display-todo") },
	{ MODKEY,                       XK_u,         spawn,       SHCMD("intlkey") },
	{ MODKEY,                       XK_v,         spawn,       SHCMD("vol 5%-") },
	{ MODKEY|ShiftMask,             XK_v,         spawn,       SHCMD("vol 5%+") },
	{ MODKEY,                       XK_y,         spawn,       SHCMD("lastscreenshot -s") },
	{ MODKEY|ShiftMask,             XK_y,         spawn,       SHCMD("lastscreenshot -g") },
	{ MODKEY|ShiftMask,             XK_BackSpace, spawn,       SHCMD("quitdwm") },
	{ MODKEY,                       XK_End,       spawn,       SHCMD("mylight -U 5") },
	{ MODKEY,                       XK_Escape,    spawn,       SHCMD("dunstctl close") },
	{ MODKEY,                       XK_Caps_Lock, spawn,       SHCMD("dunstctl close") },
	{ MODKEY,                       XK_Home,      spawn,       SHCMD("mylight -A 5") },
	{ MODKEY,                       XK_slash,     spawn,       SHCMD("touch-menu 'Rotate Screen' 'toggle'") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkStatusText,        0,              Button1,        spawn,          SHCMD("touch-menu") },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

