import rclpy
from rclpy.node import Node
from geometry_msgs.msg import TransformStamped
import meshcat
import meshcat.geometry as g
import meshcat.transformations as tf
import numpy as np


class MeshcatVisualizer(Node):
  def __init__(self):
    super().__init__("meshcat_visualizer")

    # Initialize Meshcat
    self.vis = meshcat.Visualizer()
    print(f"Meshcat URL: {self.vis.url()}")

    # Create a box (Standard Meshcat demo shape)
    # We place it at the path "moving_box"
    self.vis["moving_box"].set_object(g.Box([0.5, 0.5, 0.5]))

    self.subscription = self.create_subscription(
      TransformStamped, "/meshcat_data", self.listener_callback, 10
    )

  def listener_callback(self, msg):
    # Extract translation and rotation from ROS message
    t = msg.transform.translation
    r = msg.transform.rotation

    # Convert to Meshcat-compatible transformation matrix
    translation = [t.x, t.y, t.z]
    rotation = [r.w, r.x, r.y, r.z]  # Meshcat uses [w, x, y, z]

    # Apply the transform
    M = tf.translation_matrix(translation) @ tf.quaternion_matrix(rotation)
    self.vis["moving_box"].set_transform(M)


def main(args=None):
  rclpy.init(args=args)
  node = MeshcatVisualizer()
  try:
    rclpy.spin(node)
  except KeyboardInterrupt:
    pass
  node.destroy_node()
  rclpy.shutdown()


if __name__ == "__main__":
  main()
