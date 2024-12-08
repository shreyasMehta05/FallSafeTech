# writing a python file to read the data from the csv file and calculating the threshold value for the data
# fields in the csv file are: time, acc-x, acc-y, acc-z, gyro-x, gyro-y, gyro-z
# implementation of the fall detection algorithm
# importing the required libraries
import pandas as pd
import numpy as np
import math
import matplotlib.pyplot as plt
# sample data
# created_at,entry_id,field1,field2,field3,field4,field5,field6,field7,latitude,longitude,elevation,status
# 2024-04-14T19:36:35+00:00,1,0.01,-0.00,0.12,-0.00,-0.00,-0.00,,,,,
# 2024-04-14T19:36:50+00:00,2,-0.01,-0.00,0.13,-0.00,-0.00,-0.00,,,,,
# 2024-04-14T19:37:05+00:00,3,-0.01,-0.00,0.13,-0.00,-0.00,-0.00,,,,,
# 2024-04-14T19:37:20+00:00,4,-0.01,0.00,0.12,-0.00,-0.00,0.00,,,,,


# function to read the data from the csv file
def read_data(file_path):
    # reading the data from the csv file
    data = pd.read_csv(file_path)
    # returning the data
    return data

# store the data in the data variable
data = read_data("stair_down final.csv")
# now iterate over the data in each row and calculate the threshold value for the data
a_net_mean = 0
g_net_mean = 0
angle_mean = 0
alpha_mean = 0

a_net_std = 0
g_net_std = 0
angle_std = 0
alpha_std = 0

a_net_max = 0
g_net_max = 0
angle_max = 0
alpha_max = 0

a_net_min = 1e9
g_net_min = 1e9
angle_min = 1e9
alpha_min = 1e9
a_netList = []
g_netList = []
angleList = []
alphaList = []

for index, row in data.iterrows():
    # calculating the net acceleration
    a_net = math.sqrt(row['field1']**2 + row['field2']**2 + row['field3']**2)
    # calculating the net gyro
    g_net = math.sqrt(row['field4']**2 + row['field5']**2 + row['field6']**2)
    # calculating the rotation angle
    angle = g_net*0.5
    # calculating the alpha(angular acceleration)
    alpha = g_net/0.5
    # appending the values to the list
    a_netList.append(a_net)
    g_netList.append(g_net)
    angleList.append(angle)
    alphaList.append(alpha)
# calculating the mean of the data
a_net_mean = np.mean(a_netList)
g_net_mean = np.mean(g_netList)
angle_mean = np.mean(angleList)
alpha_mean = np.mean(alphaList)
# calculating the standard deviation of the data
a_net_std = np.std(a_netList)
g_net_std = np.std(g_netList)
angle_std = np.std(angleList)
alpha_std = np.std(alphaList)
# calculating the maximum value of the data
a_net_max = np.max(a_netList)
g_net_max = np.max(g_netList)
angle_max = np.max(angleList)
alpha_max = np.max(alphaList)
# calculating the minimum value of the data
a_net_min = np.min(a_netList)
g_net_min = np.min(g_netList)
angle_min = np.min(angleList)
alpha_min = np.min(alphaList)
# plot the data in the graph in different subplots
print("Net Acceleration Mean: ", a_net_mean)
print("Net Acceleration Standard Deviation: ", a_net_std)
print("Net Acceleration Maximum: ", a_net_max)
print("Net Acceleration Minimum: ", a_net_min)
print("Net Gyro Mean: ", g_net_mean)
print("Net Gyro Standard Deviation: ", g_net_std)
print("Net Gyro Maximum: ", g_net_max)
print("Net Gyro Minimum: ", g_net_min)
print("Rotation Angle Mean: ", angle_mean)
print("Rotation Angle Standard Deviation: ", angle_std)
print("Rotation Angle Maximum: ", angle_max)
print("Rotation Angle Minimum: ", angle_min)
print("Angular Acceleration Mean: ", alpha_mean)
print("Angular Acceleration Standard Deviation: ", alpha_std)
print("Angular Acceleration Maximum: ", alpha_max)
print("Angular Acceleration Minimum: ", alpha_min)
# write this data to data.txt file
differenceList=[]
for i in range(len(a_netList)-1):
    differenceList.append(abs(a_netList[i+1]-a_netList[i]))
differenceListAvg=np.mean(differenceList)
print("Difference List Average: ",differenceListAvg)
with open("walk.txt", "w") as file:
    file.write("Net Acceleration Mean: " + str(a_net_mean) + "\n")
    file.write("Net Acceleration Standard Deviation: " + str(a_net_std) + "\n")
    file.write("Net Acceleration Maximum: " + str(a_net_max) + "\n")
    file.write("Net Acceleration Minimum: " + str(a_net_min) + "\n")
    file.write("Net Gyro Mean: " + str(g_net_mean) + "\n")
    file.write("Net Gyro Standard Deviation: " + str(g_net_std) + "\n")
    file.write("Net Gyro Maximum: " + str(g_net_max) + "\n")
    file.write("Net Gyro Minimum: " + str(g_net_min) + "\n")
    file.write("Rotation Angle Mean: " + str(angle_mean) + "\n")
    file.write("Rotation Angle Standard Deviation: " + str(angle_std) + "\n")
    file.write("Rotation Angle Maximum: " + str(angle_max) + "\n")
    file.write("Rotation Angle Minimum: " + str(angle_min) + "\n")
    file.write("Angular Acceleration Mean: " + str(alpha_mean) + "\n")
    file.write("Angular Acceleration Standard Deviation: " + str(alpha_std) + "\n")
    file.write("Angular Acceleration Maximum: " + str(alpha_max) + "\n")
    file.write("Angular Acceleration Minimum: " + str(alpha_min) + "\n")
    file.write("Difference List Average: " + str(differenceListAvg) + "\n")
# plotting the data
plt.figure(figsize=(10, 10))
# plt.legend(["Mean", "Standard Deviation", "Maximum", "Minimum"])
plt.subplot(2, 2, 1)
plt.plot(a_netList)
plt.title("Net Acceleration")
plt.subplot(2, 2, 2)
plt.plot(g_netList)
plt.title("Net Gyro")
plt.subplot(2, 2, 3)
plt.plot(angleList)
plt.title("Rotation Angle")
plt.subplot(2, 2, 4)
plt.plot(alphaList)
plt.title("Angular Acceleration")
# add mean and standard deviation to the graph and max and min
plt.subplot(2, 2, 1)
plt.axhline(y=a_net_mean, color='r', linestyle='--')
plt.axhline(y=a_net_std, color='g', linestyle='--')
plt.axhline(y=a_net_max, color='b', linestyle='--')
plt.axhline(y=a_net_min, color='y', linestyle='--')
plt.subplot(2, 2, 2)
plt.axhline(y=g_net_mean, color='r', linestyle='--')
plt.axhline(y=g_net_std, color='g', linestyle='--')
plt.axhline(y=g_net_max, color='b', linestyle='--')
plt.axhline(y=g_net_min, color='y', linestyle='--')
plt.subplot(2, 2, 3)
plt.axhline(y=angle_mean, color='r', linestyle='--')
plt.axhline(y=angle_std, color='g', linestyle='--')
plt.axhline(y=angle_max, color='b', linestyle='--')
plt.axhline(y=angle_min, color='y', linestyle='--')
plt.subplot(2, 2, 4)
plt.axhline(y=alpha_mean, color='r', linestyle='--')
plt.axhline(y=alpha_std, color='g', linestyle='--')
plt.axhline(y=alpha_max, color='b', linestyle='--')
plt.axhline(y=alpha_min, color='y', linestyle='--')
plt.tight_layout()
# add labels to the graph
# add coloring scheme to the graph
# denote r is for mean, g is for standard deviation, b is for maximum and y is for minimum

plt.savefig("down.png")
# increase the scale of the graph along the x-axis
plt.figure(figsize=(12, 12))
plt.show()

# function to calculate the threshold value for the data


