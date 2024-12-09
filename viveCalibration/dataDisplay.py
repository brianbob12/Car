#%%

def parse_car_data(filename):
    car_positions = []
    
    with open(filename, 'r') as file:
        for line in file:
            if line.startswith('Car position'):
                # Extract x, y, and angle using string splitting
                parts = line.split(',')
                x = float(parts[0].split(':')[1])
                y = float(parts[1].split(':')[1])
                angle = float(parts[2].split(':')[1])

                if angle >0:
                    continue
                
                car_positions.append({
                    'x': x,
                    'y': y,
                    'angle': angle
                })
    
    return car_positions

#%%

filename = "viveCalibrationDataRaw.txt"
positions = parse_car_data(filename)

# Print the first few entries to verify
for pos in positions[:5]:
    print(f"Position: ({pos['x']}, {pos['y']}), Angle: {pos['angle']}")
# %%

import numpy as np
import matplotlib.pyplot as plt

def create_angle_heatmap(positions, resolution=50):
    # Extract x, y, and angles from positions
    x_coords = [p['x'] for p in positions]
    y_coords = [p['y'] for p in positions]
    angles = [p['angle'] for p in positions]
    
    # Create grid
    x_bins = np.linspace(min(x_coords), max(x_coords), resolution)
    y_bins = np.linspace(min(y_coords), max(y_coords), resolution)
    
    # Initialize grid for angles
    angle_grid = np.zeros((resolution-1, resolution-1))
    count_grid = np.zeros((resolution-1, resolution-1))
    
    # Populate the grid
    for x, y, angle in zip(x_coords, y_coords, angles):
        x_idx = np.digitize(x, x_bins) - 1
        y_idx = np.digitize(y, y_bins) - 1
        if 0 <= x_idx < resolution-1 and 0 <= y_idx < resolution-1:
            angle_grid[y_idx, x_idx] += angle
            count_grid[y_idx, x_idx] += 1
    
    # Average the angles where there are multiple measurements
    mask = count_grid > 0
    angle_grid[mask] /= count_grid[mask]
    
    # Create a masked array for areas with no data
    angle_grid = np.ma.masked_where(count_grid == 0, angle_grid)
    
    # Plot the heatmap
    plt.figure(figsize=(10, 8))
    plt.imshow(angle_grid, extent=[min(x_coords), max(x_coords), min(y_coords), max(y_coords)],
               origin='lower', aspect='auto', cmap='hsv',
               interpolation='nearest')
    # Set background color to gray for masked (no data) areas
    plt.gca().set_facecolor('gray')
    plt.colorbar(label='Angle (degrees)')
    plt.xlabel('X Position')
    plt.ylabel('Y Position')
    plt.title('Angle Distribution Heatmap')
    plt.show()

# Call the function with your data
create_angle_heatmap(positions)

# %%
