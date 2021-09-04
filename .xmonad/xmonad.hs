-- References:
-- - https://github.com/jelly/Dotfiles/blob/master/.xmonad/xmonad.hs
-- - https://wiki.haskell.org/Xmonad/Config_archive/Thomas_ten_Cate%27s_xmonad.hs
-- - https://github.com/xmonad/xmonad/blob/master/src/XMonad/Config.hs
-- - https://wiki.haskell.org/Xmonad/General_xmonad.hs_config_tips#Adding_your_own_keybindings
-- - https://ianyh.com/amethyst/
-- - https://hackage.haskell.org/package/xmonad-contrib-0.16/docs/XMonad-Config-Desktop.html#g:3

import XMonad
import XMonad.Config.Gnome
import XMonad.Hooks.ManageDocks
import qualified XMonad.StackSet as W
import qualified Data.Map as M

-- main = xmonad gnomeConfig
main =
  xmonad $ gnomeConfig
    { terminal    = myTerminal
    , modMask     = myModMask
    , layoutHook  = avoidStruts $ layoutHook defaultConfig
    , manageHook  = manageHook defaultConfig <+> manageDocks
    --, keys        = \c -> myKeys c `M.union` keys defaultConfig c
    , keys = myKeys
    , mouseBindings = myMouseBindings
    }

-- Note: mod1Mask == Alt key
myKeys :: XConfig Layout -> M.Map (KeyMask, KeySym) (X ())
myKeys conf@(XConfig {XMonad.modMask = modMask}) = M.fromList $
     -- Layout management
     [ ((myModMask1, xK_b), sendMessage ToggleStruts)
     , ((myModMask1, xK_space), sendMessage NextLayout)
     , ((myModMask1, xK_i), sendMessage Expand)
     , ((myModMask1, xK_comma), sendMessage (IncMasterN 1))
     , ((myModMask1, xK_period), sendMessage (IncMasterN (-1)))
     -- Window management
     , ((myModMask1, xK_e), windows W.focusDown)
     , ((myModMask1, xK_u), windows W.focusUp)
     , ((myModMask1, xK_n), sendMessage Shrink)
     , ((myModMask1, xK_i), sendMessage Expand)
     , ((myModMask2, xK_e), windows W.swapDown)
     , ((myModMask2, xK_u), windows W.swapUp)
     , ((myModMask1, xK_Return), windows W.swapMaster)
     , ((myModMask1, xK_t), withFocused $ windows . W.sink)
     -- Session management
     , ((mod1Mask .|. controlMask, xK_l), spawn "gnome-session-quit")
     , ((mod1Mask .|. controlMask, xK_z), spawn "systemctl suspend")
     -- Application shortcuts
     , ((mod1Mask, xK_space), spawn "dmenu_run")
     ]
     -- Workspace management
     ++
     -- ctrl-[1..9] %! Switch to workspace N
     -- mod--shift-ctrl-[1..9] %! Move client to workspace N
     [((m .|. controlMask, k), windows $ f i)
         | (i, k) <- zip (XMonad.workspaces conf) ([xK_1 .. xK_9] ++ [xK_0])
         , (f, m) <- [(W.greedyView, 0), (W.shift, myModMask2)]]

myMouseBindings :: XConfig Layout -> M.Map (KeyMask, Button) (Window -> X ())
myMouseBindings (XConfig {XMonad.modMask = modMask}) = M.fromList
    -- mod-button1 %! Set the window to floating mode and move by dragging
    [ ((myModMask1, button1), \w -> focus w >> mouseMoveWindow w
                                          >> windows W.shiftMaster)
    ]

myTerminal    = "x-terminal-emulator"
myModMask     = mod4Mask -- Win key or Super_L
myModMask1    = myModMask .|. shiftMask -- ... & Shift
myModMask2    = myModMask1 .|. controlMask -- ... & Control
