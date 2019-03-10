/* See LICENSE file for copyright and license details. */

/* appearance */
static const char *fonts[] = {
	"monospace:size=10"
};
static const char dmenufont[]       = "monospace:size=10";
static const char normbordercolor[] = "#444444";
static const char normbgcolor[]     = "#222222";
static const char normfgcolor[]     = "#bbbbbb";
static const char selbordercolor[]  = "#005577";
static const char selbgcolor[]      = "#005577";
static const char selfgcolor[]      = "#eeeeee";
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	//{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "columns.c"
#include "horizgrid.c"
#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "###",      horizgrid },
 	{ "[@]",      spiral },
 	{ "[\\]",     dwindle },
	{ "|||",      col },
	{ "><>",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask /* Win key or Super_L */
#define MyModMask1 MODKEY|ShiftMask
#define MyModMask2 MyModMask1|ControlMask
// Note: I don't use the last 2, ATM
#define TAGKEYS(KEY,TAG) \
	{ ControlMask,                  KEY,      view,           {.ui = 1 << TAG} }, \
	{ MyModMask2,                   KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MyModMask1,                   KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ Mod1Mask,                     XK_space,  spawn,          {.v = dmenucmd } },
	{ MyModMask1,                   XK_Return, spawn,          {.v = termcmd } },
	{ Mod1Mask|ControlMask,         XK_l,      spawn,          SHCMD("xscreensaver-command -lock") },
	{ Mod1Mask|ControlMask,         XK_z,      spawn,          SHCMD("xscreensaver-command -lock && systemctl suspend") },
	{ MyModMask1,                   XK_b,      togglebar,      {0} },
	{ MyModMask1,                   XK_e,      focusstack,     {.i = +1 } },
	{ MyModMask1,                   XK_u,      focusstack,     {.i = -1 } },
	{ MyModMask1,                   XK_comma,  incnmaster,     {.i = +1 } },
	{ MyModMask1,                   XK_period, incnmaster,     {.i = -1 } },
	{ MyModMask1,                   XK_n,      setmfact,       {.f = -0.05} },
	{ MyModMask1,                   XK_i,      setmfact,       {.f = +0.05} },
	{ MyModMask2,                   XK_e,      movestack,      {.i = +1 } },
	{ MyModMask2,                   XK_u,      movestack,      {.i = -1 } },
	//{ MODKEY,                       XK_Return, zoom,           {0} },
	//{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MyModMask2,                   XK_q,      killclient,     {0} },
	{ MyModMask1,                   XK_l,      setlayout,      {.v = &layouts[0]} },
	{ MyModMask1,                   XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MyModMask1,                   XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MyModMask1,                   XK_h,      setlayout,      {.v = &layouts[3]} },
	{ MyModMask1,                   XK_s,      setlayout,      {.v = &layouts[3]} },
	{ MyModMask1,                   XK_d,      setlayout,      {.v = &layouts[3]} },
	{ MyModMask1,                   XK_c,      setlayout,      {.v = &layouts[3]} },
	//{ MODKEY,                       XK_space,  setlayout,      {0} },
  { MyModMask1,                   XK_space,  cyclelayout,    {.i = +1 } },
  { MyModMask2,		                XK_space,  cyclelayout,    {.i = -1 } },
	{ MyModMask1,                   XK_t,      togglefloating, {0} },
	//{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MyModMask2,                   XK_0,      tag,            {.ui = ~0 } },
  // Note: I don't use these ATM..
	{ MODKEY,                       XK_z,      focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_x,      focusmon,       {.i = +1 } },
	{ MODKEY,                       XK_b,      tagmon,         {.i = -1 } },
	{ MODKEY,                       XK_k,      tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MyModMask2|Mod1Mask,          XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

