// RocketVisualization.qml
import QtQuick 2.15
import Qt3D.Core 2.15
import Qt3D.Render 2.15
import Qt3D.Input 2.15
import Qt3D.Extras 2.15
import QtQuick.Scene3D 2.15

Item {
    id: root

    // Property to receive telemetry data from parent
    property var telemetry: ({})

    // Track when telemetry changes to update the 3D model
    onTelemetryChanged: {
        if (rocketTransform) {
            rocketTransform.rotation = Qt.quaternion(
                telemetry.pitch * Math.PI / 180,
                telemetry.yaw * Math.PI / 180,
                telemetry.roll * Math.PI / 180
            )
        }
    }

    Scene3D {
        id: scene3d
        anchors.fill: parent
        aspects: ["input", "logic", "render"]
        multisample: true

        Entity {
            id: rootEntity

            // Camera
            Entity {
                id: cameraEntity

                Camera {
                    id: camera
                    projectionType: CameraLens.PerspectiveProjection
                    fieldOfView: 45
                    nearPlane: 0.1
                    farPlane: 1000.0
                    position: Qt.vector3d(0.0, 0.0, 5.0)
                    upVector: Qt.vector3d(0.0, 1.0, 0.0)
                    viewCenter: Qt.vector3d(0.0, 0.0, 0.0)
                }

                components: [camera]
            }

            // Controller for camera input
            OrbitCameraController {
                camera: camera
            }

            // Light
            Entity {
                id: lightEntity

                DirectionalLight {
                    id: directionalLight
                    color: Qt.rgba(1.0, 1.0, 1.0, 1.0)
                    intensity: 1.0
                    worldDirection: Qt.vector3d(-1.0, -1.0, -1.0)
                }

                components: [directionalLight]
            }

            // Rocket entity
            Entity {
                id: rocketEntity

                Transform {
                    id: rocketTransform
                    scale: 0.5
                    rotation: Qt.quaternion(0, 0, 0, 1)
                    translation: Qt.vector3d(0, 0, 0)
                }

                SceneLoader {
                    id: rocketLoader
                    source: "qrc:/resources/models/rocket.obj"
                    onStatusChanged: {
                        if (status == SceneLoader.Ready) {
                            console.log("Model loaded successfully")

                            // Apply wireframe material to all model entities
                            applyWireframeToModel(rocketLoader.entity)
                        }
                    }
                }

                components: [rocketTransform, rocketLoader]
            }

            // Scene configuration
            components: [
                RenderSettings {
                    activeFrameGraph: ForwardRenderer {
                        camera: camera
                        clearColor: "black"
                    }
                },
                InputSettings {}
            ]
        }
    }

    // Function to apply wireframe material to all entities in the model
    function applyWireframeToModel(entity) {
        if (entity) {
            // Get all child entities
            var children = entity.childNodes
            for (var i = 0; i < children.length; ++i) {
                var child = children[i]

                // Check if the child is a mesh entity
                var meshComponent = child.componentsOfType(Mesh)
                if (meshComponent.length > 0) {
                    // Create and add wireframe material
                    var wireframeMaterial = wireframeMaterialComponent.createObject(child)
                    child.addComponent(wireframeMaterial)

                    // Remove any existing material
                    var materials = child.componentsOfType(Material)
                    for (var j = 0; j < materials.length; ++j) {
                        if (materials[j] !== wireframeMaterial) {
                            child.removeComponent(materials[j])
                        }
                    }
                }

                // Recursively process children
                applyWireframeToModel(child)
            }
        }
    }

    // Component for creating wireframe materials
    Component {
        id: wireframeMaterialComponent

        WireframeMaterial {}
    }
}
