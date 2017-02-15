# Release Notes

## 1.5 (12 April 2010)
  * Added the Projection property to GLView3D.
  * Added the Camera property to GLView3D.
  * Added ModelOrigin and ModelSize properties to GLView3D.
  * Added CreatePickBox, RemovePickBox and ClearPickBoxes methods to GLView3D for object selection.
  * Slice and stack count in sphere and cylinder rendering functions in GLGraphics3D can now be customized.

## 1.4 (18 March 2010)
  * Added camera panning to GLCanvas3D. Panning is activated by holding down the control key while dragging with the mouse.
  * Added the crossing selection option to HitTest functions.

## 1.3 (10 March 2010)
  * GLCanvas3D now supports mouse selection. Added SelectionMode, SelectionColor, and ReverseSelectionColor properties.
  * Added hit testing functions to GLCanvas3D. To use selection, create a dummy invisible selection box with the GLGraphics3D.CreatePickBox method. You can then use GLCanvas3D.HitTest methods to test whether a point or window (in client coordinates) intersects with your pick boxes.

## 1.2 (08 February 2010)
  * Modified GetCirclePrecision to return fewer line segments for large arcs (Issue 1).
  * Fixed a bug in vertex array renderer, where the transformation matrix was not reset before rendering (Issue 3).
  * Text arrays are now cleaned up properly after render (Issue 4).
  * *Breaking Change* Events now use a single event argument object instead of a variable number of arguments (Issue 5).
  * String handles are now properly freed after rendering (Issue 6).
  * *Breaking Change* GLGraphics is renamed to GLGraphics2D (Issue 7).
  * Added rounded rectangle routines to GLGraphics2D (Issue 8). Contributed by Christian von Nathusius.
  * Added XML documentation for all public fields (Issue 9).
  * Added license texts (Issue 10).

## 1.1 (08 February 2010)
  * Initial release
