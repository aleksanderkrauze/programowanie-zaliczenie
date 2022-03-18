import glob, os, IPython.display as IPdisplay
from PIL import Image

save_folder = "./"
working_folder = "plots"
gif_filename = "animation"
# load all the static images into a list then save as an animated gif
gif_filepath = '{}/{}.gif'.format(save_folder, gif_filename)
fileNames = sorted(glob.glob('{}/*.png'.format(working_folder)))
print("fileNames")
print(fileNames)
images = [Image.open(image) for image in fileNames]
gif = images[0]
gif.info['duration'] = 75 #milliseconds per frame
gif.info['loop'] = 0 #how many times to loop (0=infinite)
gif.save(fp=gif_filepath, format='gif', save_all=True, append_images=images[1:])
IPdisplay.Image(url=gif_filepath)
