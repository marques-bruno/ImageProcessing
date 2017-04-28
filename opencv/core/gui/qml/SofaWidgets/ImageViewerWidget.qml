import QtQuick 2.5
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import SofaBasics 1.0
import SofaApplication 1.0
import SofaData 1.0
import SofaWidgets 1.0
import ImageViewerModel 1.0
import ImageViewerDisplay 1.0

Item {
    id: root
    implicitWidth: layout.implicitWidth
    implicitHeight: layout.implicitHeight
    enabled: sofaScene ? sofaScene.ready : false

    property var indexImageRenderers: 0
    property var imageRenderer: null
    property var sofaScene: SofaApplication.sofaScene
    property bool configurable: true
    property var comboList: ListModel {
        id: imageitems
    }

    ImageViewerModel {
        id: imagemodel

        //sofaComponent: sofaScene.componentByType("SurfaceXRayRenderer");
        sofaComponent: sofaScene.componentsByType("ImageFilter").at(indexImageRenderers);
    }

    function updateIndex(index)
    {
        //console.log("updateIndex() : " + index);
        imagemodel.sofaComponent = (sofaScene.componentsByType("ImageFilter").at(index));
    }

    Component.onCompleted: {
           var models = sofaScene.componentsByType("ImageFilter")
           comboList.clear()
           for (var i = 0; i < models.size(); ++i)
           {
                comboList.append({text: models.at(i).name()})
                //console.log(models.at(i).name())
           }
    }

    ColumnLayout {
        id: layout
        anchors.fill: parent
        spacing: 0

        ImageViewerDisplay {
            id: imageViewer
            anchors.centerIn: parent
            smooth: false
            Layout.fillWidth: true
            Layout.fillHeight: true
            width: 500
            height: 500
            imageViewerModel: imagemodel

            Component.onCompleted: update();

            Connections {
                //target: root.xraymodel.sofaComponent ? root.sofaComponent.sofaScene() : null
                //onStepEnd: xrayRenderer.update()
            }

            MouseArea {
                anchors.fill: imageViewer
                acceptedButtons: Qt.LeftButton
                propagateComposedEvents: true

                onClicked: {
                    //xrayViewer.imageViewerModel.updateViewer()
                    // console.log(imageViewer.width);
                    // console.log(root.comboList.count);
                    // xrayViewer.update();
                }

                onWheel: {
                }
            }
        }
    }

    // ////Tool panel
    Rectangle {
        id: toolPanel
        color: "lightgrey"
        anchors.top: toolPanelSwitch.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.topMargin: -6
        anchors.bottomMargin: 20
        anchors.rightMargin: -radius
        width: 250
        radius: 5
        visible: false
        opacity: 0.9

        // avoid mouse event propagation through the toolpanel to the sofa viewer
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.AllButtons
            onWheel: wheel.accepted = true
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: toolPanel.radius / 2
            anchors.rightMargin: anchors.margins - toolPanel.anchors.rightMargin
            spacing: 2

            Text {
                Layout.fillWidth: true
                text: "ImageViewer parameters"
                font.bold: true
                color: "darkblue"
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                Flickable {
                    anchors.fill: parent
                    contentHeight: panelColumn.implicitHeight

                    Column {
                        id: panelColumn
                        anchors.fill: parent
                        spacing: 5

                        GroupBox {
                            id: savePanel
                            implicitWidth: parent.width

                            title: "Misc"

                            GridLayout {
                                anchors.fill: parent
                                columnSpacing: 2
                                rowSpacing: 2
                                columns: 2

                                RowLayout {
                                    Layout.fillWidth: true

                                    Label {
                                        text: "Index "
                                    }

                                    ComboBox {
                                        id: indexViewerComboBox
                                        Layout.fillWidth: true

                                        model: root.comboList

                                        onCurrentIndexChanged : switchImageRenderer();

                                        function switchImageRenderer() {
                                            //onsole.log("Change to " + indexViewerComboBox.currentIndex);
                                            root.updateIndex(indexViewerComboBox.currentIndex);
                                            //root.xraymodel.update()
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Image {
        id: toolPanelSwitch
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 26
        anchors.rightMargin: 3
        source: toolPanel.visible ? "qrc:/icon/minus.png" : "qrc:/icon/plus.png"
        width: 12
        height: width
        visible: configurable

        MouseArea {
            anchors.fill: parent
            propagateComposedEvents: true
            onClicked: toolPanel.visible = !toolPanel.visible
        }
    }
}
