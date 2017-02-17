import numpy as np
import os
import h5py
import sys

from PIL import Image

def get_dataset(dataset_name):
    directory = os.path.dirname(os.path.realpath(__file__))

    files_in_current_folder = os.listdir( directory )
    
    for item in files_in_current_folder:
        if item == dataset_name:
            try:
                path_data = os.path.join( directory , dataset_name )
                path_data_images = os.path.join(path_data,'images')
                
                listing = os.listdir( path_data_images)
                listing.sort() 
                num_samples = len(listing)
                print("Dataset folder:", path_data)
                print("Number of samples:", num_samples)
                
                image = np.array( Image.open(path_data_images + "/" + listing[0]) )
                height, width, channels = image.shape
                print("Image properties: Height: %d Width: %d Channels: %d" % (height,width,channels))
                
                image_matrix = np.array([ np.array( Image.open( path_data_images + "/" + image_i )).flatten()
                                        for image_i in listing],'f')
                
                
                label = np.ones( (num_samples,2),dtype = int)
                ''' data.txt = image name, motor speed, steering value '''
                label_file = open(path_data+'/data.txt','r')
                for i,line in enumerate(label_file):
                    line = line.replace('\n','')
                    line_data = line.split(' ')
                    label[i][0] = line_data[1]
                    label[i][1] = line_data[2] #steering value
                
                train_data = [image_matrix, label]
                return train_data
            except FileNotFoundError:
                print("error")
            else:
                print("Folder with name: " + dataset_name + " not found!")


def main(args):
    print("Temporal Insanity Dataset Creation")
    if(len(args) > 1) :
        print(args[1])
    
        get_dataset(args[1])
    else:
        print("Warning: No folder argument found")
        folder_name = input("Name of dataset folder? : ")
        get_dataset(folder_name)

if __name__ == '__main__':
    main(sys.argv)
  