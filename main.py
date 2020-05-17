import tornado.httpclient
import json

from datetime import datetime
from matplotlib import pyplot
from matplotlib.animation import FuncAnimation

x_data, y_data = [], []
figure = pyplot.figure()
line, = pyplot.plot_date(x_data, y_data, '-')


def update(frame):
    x_data.append(datetime.now())
    response = do_http_request()
    y_data.append(response['analog'][0])
    if len(x_data) > 1000:
        x_data.pop(0)
        y_data.pop(0)

    if len(x_data) == len(y_data):
        line.set_data(x_data, y_data)
    else:
        x_data.clear()
        y_data.clear()

    figure.gca().relim()
    figure.gca().autoscale_view()

    with open('./Collected_Data.json', 'a') as outfile:
        json.dump(response, outfile)
        outfile.write(",\n")

    return line,


def do_http_request():
    http_client = tornado.httpclient.HTTPClient()
    try:
        response = http_client.fetch("http://192.168.1.3")
        response_body = response.body.strip()
        print(response_body)
        print("Try parse JSON:")
        loaded_json = json.loads(response_body)
        print(loaded_json)

    except tornado.httpclient.HTTPError as e:
        # HTTPError is raised for non-200 responses; the response
        # can be found in e.response.
        print("Error: " + str(e))

    except Exception as e:
        # Other errors are possible, such as IOError.
        print("Error: " + str(e))
    http_client.close()
    return loaded_json


if __name__ == '__main__':
    animation = FuncAnimation(figure, update, interval=200)
    pyplot.show()