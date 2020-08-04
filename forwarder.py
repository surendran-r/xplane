import websocket
import xpc
import socket

try:
    import thread
except ImportError:
    import _thread as thread
import time

class Forwarder:
    def __init__(self):
        self.client = None
        self.ws = None

    def keepForwarding(self):
        self.ws.run_forever()

    def connectToController(self):
        self.ws = websocket.WebSocketApp("ws://192.168.0.162:81",
                                on_open=lambda ws: self.on_open(ws), # <--- This line is changed
                                on_message=lambda ws, msg: self.on_message (ws, msg),
                                on_error=lambda ws, error: self.on_error(ws, error), # Omittable (msg -> error)
                                on_close=lambda ws: self.on_close(ws))
        print("Connected to Controller")

    def on_message(self, ws, message):
        print("Msg: " ,message)
        vals = message.split()
        ctrl = [-998, -998, -998, -998]
        if len(vals) > 1:
            valType = vals[0]
            val = vals[1]
            if valType == 'rotator1:1' :
                speed = float(vals[1]);
                if speed < 0:
                    speed = 0
                if speed > 70:
                    speed = 70
                ctrl[3] = speed /70.0
            elif valType == 'rotator1:' :
                elev = float(vals[1]);
                if elev < -20:
                    elev = -20
                if elev > 20:
                    elev = 20
                ctrl[0] = elev / 20.0
            elif valType == 'rotator2:' :
                aleron = -1 * float(vals[1]);
                if aleron < -30:
                    aleron = -30
                if aleron > 30:
                    aleron = 30
                ctrl[1] = aleron / 30.0
            if self.client != None :
                # Set control surfaces and throttle of the player aircraft using sendCTRL
                print("Setting controls to ", ctrl)
                self.client.sendCTRL(ctrl)

    def on_error(self, ws, error):
        print(error)

    def on_close(self, ws):
        print("### closed ###")

    def on_open(self, ws):
    	print("### opened ###")
        # def run(*args):
        #     for i in range(3):
        #         time.sleep(1)
        #         ws.send("Hello %d" % i)
        #     time.sleep(1)
        #     ws.close()
        #     print("thread terminating...")
        # thread.start_new_thread(run, ())


    def connectToXPlane(self):
        self.client = xpc.XPlaneConnect('localhost', port=48000, timeout=1000)
        # Verify connection
        try:
            # If X-Plane does not respond to the request, a timeout error
            # will be raised.
            self.client.getDREF("sim/test/test_float")
            print("Connected to XPlane")
        except Exception as e:
            print("Error establishing connection to X-Plane.", e)
            print("Exiting...")
            return


if __name__ == "__main__":

    websocket.enableTrace(False)
    worker = Forwarder()
    worker.connectToXPlane()
    worker.connectToController()
    worker.keepForwarding()



