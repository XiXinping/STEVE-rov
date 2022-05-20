import time
import matplotlib.pyplot as plt
import numpy as np


class PID:
    last_time = None
    last_error = None
    error_sum = 0

    def __init__(self, set_point, proportional_gain, integral_gain,
                 derivative_gain):
        self.set_point = set_point
        self.proportional_gain = proportional_gain
        self.integral_gain = integral_gain
        self.derivative_gain = derivative_gain

        self.last_time = time.time()
        self.last_error = set_point

    def compute(self, process_value):
        current_time = time.time()
        d_time = time.time() - self.last_time
        print(f"Change in Time: {d_time}")
        self.last_time = current_time

        error = self.set_point - process_value
        print(f"Error: {error}")
        # compute the integral
        self.error_sum += error * d_time
        print(f"Error: {self.error_sum}")
        # compute the derivative
        d_error = (error - self.last_error) / d_time
        print(f"d_error: {d_error}")
        print(f"Adjusted d_error: {d_error * self.derivative_gain}")
        self.last_error = error

        output = (self.proportional_gain * error + self.integral_gain
                  * self.error_sum + self.derivative_gain * d_error)
        print(f"Output: {output}")
        return output


class Plant:
    plant_state = 0

    def __init__(self, initial_state):
        self.plant_state = initial_state

    def process(self, actuating_signal):
        self.plant_state -= 10
        self.plant_state += actuating_signal
        return self.plant_state


def main():
    set_point = 100
    kp = 0.5
    ki = 10
    kd = 0.004
    process_value = 0

    pid_controller = PID(set_point, kp, ki, kd)
    plant = Plant(0)

    # for i in range(50):
    # time.sleep(0.01)
    # actuating_signal = pid_controller.compute(process_value)
    # print(f"Process Value Before: {process_value}")
    # process_value = plant.process(actuating_signal)
    # print(f"Process Value After: {process_value}")
    # print()

    plt.style.use('_mpl-gallery')

    # make data
    # np.random.seed(1)
    # x = np.linspace(0, 8, 16)
    # y1 = 3 + 4*x/8 + np.random.uniform(0.0, 0.5, len(x))
    # y2 = 1 + 2*x/8 + np.random.uniform(0.0, 0.5, len(x))

    # # plot
    fig, ax = plt.subplots()

    # ax.plot(x, (y1 + y2)/2, linewidth=2)

    # ax.set(xlim=(0, 8), xticks=np.arange(1, 8),
    # ylim=(0, 8), yticks=np.arange(1, 8))
    ax.plot([1, 2, 3, 4], [1, 4, 9, 16])

    plt.show()


if __name__ == "__main__":
    main()
