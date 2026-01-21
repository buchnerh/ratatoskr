# UBports ContentHub API reference documents
(pan, v0, 21/01/2026)

## Regular apps should not access `Documents/` folder directly

From the definition of [document-files](https://docs.ubports.com/no/latest/appdev/platform/apparmor.html#document-files)
AppArmor policy group:

> This policy group is reserved for certain applications, such as document 
  viewers. Developers should typically use the content_exchange policy group 
  and API to access document files instead.

## Workflow for interaction with ContentHub

Based on the "Export" workflow described on [this page](https://docs.ubports.com/en/latest/appdev/guides/contenthub.html)
our `Main.qml` has to be expanded to support the "Share" and "Download" icons
(in addition to the existing "Delete")

The [ContentHub QML Type](https://ubports.gitlab.io/docs/api-docs/?p=lomiri-content-qml-api%2Fqml-lomiri-content-contenthub.html#detailed-description)
page contains more information and example code.

Please also refer to [ContentHandler QML Type](https://ubports.gitlab.io/docs/api-docs/?p=lomiri-content-qml-api%2Fqml-lomiri-content-contenthandler.html#detailed-description).
