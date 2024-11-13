/* See LICENSE file for copyright and license details. */
#include "movestack.c"
/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[] = {
    "monospace:size=11:antialias=true:hinting=true",
    "FiraCode Nerd Font:size=11:antialias=true:autohint=true"};

static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* Audio controls */
#include <X11/XF86keysym.h>
static const char *upvol[]   = { "/usr/bin/wpctl", "set-volume", "@DEFAULT_AUDIO_SINK@", "10%+", NULL };
static const char *downvol[] = { "/usr/bin/wpctl", "set-volume", "@DEFAULT_AUDIO_SINK@", "10%-", NULL };
static const char *mutevol[] = { "/usr/bin/wpctl", "set-mute",   "@DEFAULT_AUDIO_SINK@", "toggle", NULL };

/* Brightness control */
static const char *brightness_up[]   = { "brightnessctl", "set", "5%+", NULL };
static const char *brightness_down[] = { "brightnessctl", "set", "5%-", NULL };

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "alacritty", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */

	/* Brightness control */
	{ MODKEY,                       XK_w,      spawn,          {.v = brightness_up } },   // Increase brightness
	{ MODKEY,                       XK_s,      spawn,          {.v = brightness_down } }, // Decrease brightness

	/* Volume controls */
	{ MODKEY|ShiftMask,             XK_w,      spawn,          {.v = upvol } },   // Increase volume
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = downvol } }, // Decrease volume

	/* Launching applications */
	{ MODKEY,                       XK_e,      spawn,          SHCMD("zoom") }, // Launch Zoom
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } }, // Launch terminal

	/* Bar and window management */
	{ MODKEY,                       XK_b,      togglebar,      {0} },           // Toggle bar visibility
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },    // Focus next window
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },    // Focus previous window
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },   // Move window down
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },   // Move window up
	{ MODKEY,                       XK_u,      incnmaster,     {.i = +1 } },    // Increase number of master clients
	{ MODKEY,                       XK_i,      incnmaster,     {.i = -1 } },    // Decrease number of master clients
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },  // Decrease master area size
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },  // Increase master area size
	{ MODKEY,                       XK_Tab,    view,           {0} },           // Switch to last viewed tag
	{ MODKEY,                       XK_q,      killclient,     {0} },           // Close focused window

	/* Layout management */
	{ MODKEY|ShiftMask              XK_space,  setlayout,      {.v = &layouts[0]} }, // Set tile layout
	{ MODKEY,                       XK_a,      setlayout,      {.v = &layouts[1]} }, // Set floating layout
	{ MODKEY,                       XK_space,  setlayout,      {.v = &layouts[2]} }, // Set monocle layout
	//{ MODKEY,                       XK_space,  setlayout,      {0} },           // Toggle between layouts
	{ MODKEY|ShiftMask,             XK_f,      togglefloating, {0} },         // Toggle floating mode
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },          // Toggle fullscreen mode

	/* Monitor management */
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },   // View all tags
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },   // Tag all clients
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },    // Focus previous monitor
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },    // Focus next monitor
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },    // Tag previous monitor
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },    // Tag next monitor
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },

	/* Tag management */
	TAGKEYS(                        XK_1,                      0), // Tag 1
	TAGKEYS(                        XK_2,                      1), // Tag 2
	TAGKEYS(                        XK_3,                      2), // Tag 3
	TAGKEYS(                        XK_4,                      3), // Tag 4
	TAGKEYS(                        XK_5,                      4), // Tag 5
	TAGKEYS(                        XK_6,                      5), // Tag 6
	TAGKEYS(                        XK_7,                      6), // Tag 7
	TAGKEYS(                        XK_8,                      7), // Tag 8
	TAGKEYS(                        XK_9,                      8), // Tag 9

	/* Quit */
	{ MODKEY|ShiftMask,             XK_e,      quit,           {0} }, // Quit dwm
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} }, // restart config
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
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
