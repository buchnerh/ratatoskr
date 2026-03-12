# Sprint #013: scope
(pan, v0, 12/03/2026)

Fix [issue #5](https://github.com/petroniusniger/ratatoskr/issues/5):
Add confirmation pop-up before deleting received file from ItemList.

In the main 'ratatoskr' app, received files are stored in a cache folder that 
is specific to the app. From there, they can be "downloaded" (copied, in fact)
to standard, user-visible folders or shared with other suitable apps.

Once downloaded or shared, the user can remove the local copy located in the 
cache by performing a right swipe and tapping on the 'delete' icon. Before
letting the app actually delete that file, a confirmation pop-up should be 
displayed, allowing the user to confirm or cancel the operation.

The dialog should contain the following:

- the name of the file about to be deleted (basename, not full path)
- an "OK" button
- a "Cancel" button

If the "OK" button is pressed, the app proceeds with file deletion. If the 
"Cancel" button is pressed, the operation is aborted and the user is returned
to the app main screen.

In terms of design, please replicate the one shown in the 
[screenshot](./screenshot20260312_102633023_small.png) taken from the 'File 
Manager' app.
