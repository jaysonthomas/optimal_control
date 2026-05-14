import rclpy
from rclpy.node import (
    Node,
)
from std_msgs.msg import (
    Float64MultiArray,
)

import threading
import webbrowser

from bokeh.server.server import Server
from bokeh.plotting import figure
from bokeh.layouts import column
from bokeh.models import ColumnDataSource
from bokeh.application import Application
from bokeh.application.handlers.function import FunctionHandler

# Global reference to Bokeh document/source
bokeh_doc = None
source = None


class RosSubscriber(Node):
  def __init__(self):
    super().__init__("bokeh_plotter_sub")

    self.subscription = self.create_subscription(
      Float64MultiArray, "sim_data", self.listener_callback, 10)

    def listener_callback(self, msg):
      global bokeh_doc, source

      if bokeh_doc is None or source is None:
        return

      new_data = {
        "x": [msg.data[0]],
        "y1": [msg.data[1]],
        "y2": [msg.data[2]],
        "u": [msg.data[3]],
      }

      # Schedule update on Bokeh's main thread
      def update():
          source.stream(
              new_data,
              rollover=1000,
          )

      bokeh_doc.add_next_tick_callback(update)


def make_document(doc):
    global bokeh_doc, source
    bokeh_doc = doc

    source = ColumnDataSource(data={"x": [], "y1": [], "y2": [], "u": []})

    # Position / Velocity plot
    p1 = figure(title="States (Position & Velocity)", x_axis_label="Time (s)",
                width=800, height=400, y_range=(-2, 2))

    p1.line("x", "y1", source=source, legend_label="Position",
            color="navy", line_width=2)

    p1.line("x", "y2", source=source, legend_label="Velocity", 
            color="firebrick", line_width=2)

    p2 = figure(title="Control Effort", x_axis_label="Time (s)",
                width=800, height=300, x_range=p1.x_range)

    p2.line("x", "u", source=source, legend_label="Control (u)",
            color="green", line_width=2)

    doc.add_root(column(p1, p2))

def run_ros():
    rclpy.init()
    node = RosSubscriber()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    # Start ROS thread
    threading.Thread(
        target=run_ros,
        daemon=True,
    ).start()

    # Bokeh app
    bokeh_app = Application(FunctionHandler(make_document))

    server = Server(
        {"/": bokeh_app},
        port=5006,
        allow_websocket_origin=["localhost:5006"],
    )

    server.start()

    print("Opening Bokeh plot at http://localhost:5006")

    server.io_loop.add_callback(
        webbrowser.open,
        "http://localhost:5006",
    )

    server.io_loop.start()
