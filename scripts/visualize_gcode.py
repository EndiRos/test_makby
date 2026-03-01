"""
G-code visualizer to verify pattern generation
"""
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import re


def parse_gcode(filename):
    """Parse G-code file and extract movement paths"""
    regions = {}
    current_region = None
    current_x, current_y = 0, 0
    
    with open(filename, 'r') as f:
        for line in f:
            line = line.strip()
            
            # Detect region comments
            if line.startswith(';') and any(keyword in line for keyword in 
                ['Rectangle', 'Triangle', 'Intersection', 'Triple', 'Rect']):
                current_region = line[2:].strip()
                if current_region not in regions:
                    regions[current_region] = {'moves': [], 'draws': []}
                continue
            
            # Parse G0 (move) and G1 (draw) commands
            if line.startswith('G0') or line.startswith('G1'):
                x_match = re.search(r'X([-\d.]+)', line)
                y_match = re.search(r'Y([-\d.]+)', line)
                
                if x_match and y_match:
                    new_x = float(x_match.group(1))
                    new_y = float(y_match.group(1))
                    
                    if current_region:
                        if line.startswith('G0'):
                            regions[current_region]['moves'].append([(current_x, current_y), (new_x, new_y)])
                        elif line.startswith('G1'):
                            regions[current_region]['draws'].append([(current_x, current_y), (new_x, new_y)])
                    
                    current_x, current_y = new_x, new_y
    
    return regions


def plot_gcode(regions):
    """Plot G-code paths with different colors for each region"""
    
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 8))
    
    # Define colors for different patterns
    colors = {
        'Rectangle 1': '#FF6B6B',
        'Rectangle 2': '#4ECDC4',
        'Triangle': '#45B7D1',
        'Rect1 ∩ Rect2': '#FFA07A',
        'Rect1 ∩ Triangle': '#98D8C8',
        'Triple Intersection': '#F7DC6F',
    }
    
    # Left plot: All patterns combined
    ax1.set_title('Combined G-code Output', fontsize=14, fontweight='bold')
    ax1.set_xlabel('X (mm)')
    ax1.set_ylabel('Y (mm)')
    ax1.grid(True, alpha=0.3)
    ax1.set_aspect('equal')
    
    # Plot original geometries as reference
    rect1 = patches.Polygon([[5, 12], [53, 12], [53, 24], [5, 24]], 
                            fill=False, edgecolor='red', linewidth=2, 
                            linestyle='--', label='Rectangle 1')
    rect2 = patches.Polygon([[16, 29], [40.213, -9], [54.483, -1], [29, 37]], 
                            fill=False, edgecolor='cyan', linewidth=2, 
                            linestyle='--', label='Rectangle 2')
    triangle = patches.Polygon([[8, 4], [43, 4], [27, 21]], 
                               fill=False, edgecolor='blue', linewidth=2, 
                               linestyle='--', label='Triangle')
    
    ax1.add_patch(rect1)
    ax1.add_patch(rect2)
    ax1.add_patch(triangle)
    
    # Plot all drawing paths
    for region_name, paths in regions.items():
        color = None
        for key, col in colors.items():
            if key in region_name:
                color = col
                break
        if color is None:
            color = '#95A5A6'
        
        for path in paths['draws']:
            start, end = path
            ax1.plot([start[0], end[0]], [start[1], end[1]], 
                    color=color, linewidth=0.8, alpha=0.7)
    
    ax1.legend(loc='upper left')
    
    # Right plot: Individual patterns with labels
    ax2.set_title('Pattern Details', fontsize=14, fontweight='bold')
    ax2.set_xlabel('X (mm)')
    ax2.set_ylabel('Y (mm)')
    ax2.grid(True, alpha=0.3)
    ax2.set_aspect('equal')
    
    legend_handles = []
    pattern_stats = []
    
    for region_name, paths in regions.items():
        if not paths['draws']:
            continue
        
        color = None
        for key, col in colors.items():
            if key in region_name:
                color = col
                break
        if color is None:
            color = '#95A5A6'
        
        # Plot paths
        for path in paths['draws']:
            start, end = path
            line, = ax2.plot([start[0], end[0]], [start[1], end[1]], 
                           color=color, linewidth=1.2, alpha=0.8)
        
        # Create legend entry
        if paths['draws']:
            legend_handles.append(plt.Line2D([0], [0], color=color, linewidth=2, label=region_name))
            pattern_stats.append(f"{region_name}: {len(paths['draws'])} lines")
    
    ax2.legend(handles=legend_handles, loc='upper left', fontsize=8)
    
    # Add statistics text
    stats_text = '\n'.join(pattern_stats)
    fig.text(0.02, 0.02, f"Pattern Statistics:\n{stats_text}", 
             fontsize=8, family='monospace', 
             bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5))
    
    plt.tight_layout()
    output_file = 'gcode_visualization.png'
    plt.savefig(output_file, dpi=150, bbox_inches='tight')
    print(f"Visualization saved to {output_file}")
    
    # Try to open the image
    import subprocess
    import os
    try:
        if os.path.exists(output_file):
            # Try different image viewers
            viewers = ['eog', 'display', 'feh', 'xdg-open', 'firefox']
            for viewer in viewers:
                try:
                    subprocess.Popen([viewer, output_file])
                    print(f"Opening with {viewer}...")
                    break
                except FileNotFoundError:
                    continue
            else:
                print(f"\nTo view the image, open: {os.path.abspath(output_file)}")
    except Exception as e:
        print(f"Could not auto-open image: {e}")
        print(f"Open manually: {os.path.abspath(output_file)}")
    
    # Don't call plt.show() - it blocks in non-GUI environments


def print_statistics(regions):
    """Print detailed statistics about the generated patterns"""
    print("\n" + "="*60)
    print("G-CODE VERIFICATION REPORT")
    print("="*60)
    
    print("\nDetected regions:")
    for region_name in regions.keys():
        print(f"  - '{region_name}'")
    
    total_lines = 0
    for region_name, paths in regions.items():
        num_lines = len(paths['draws'])
        num_moves = len(paths['moves'])
        total_lines += num_lines
        
        if num_lines > 0 or num_moves > 0:
            print(f"\n{region_name}:")
            print(f"  - Drawing lines: {num_lines}")
            print(f"  - Move commands: {num_moves}")
            
            # Analyze pattern type from name
            if 'Rectilinear 0' in region_name:
                print(f"  ✓ Pattern: Rectilinear (0° rotation)")
            elif 'Rectilinear 90' in region_name:
                print(f"  ✓ Pattern: Rectilinear (90° rotation)")
            elif 'Rectilinear 120' in region_name:
                print(f"  ✓ Pattern: Rectilinear (120° rotation)")
            elif 'Honeycomb' in region_name:
                print(f"  ✓ Pattern: Honeycomb")
            elif 'Spiral' in region_name:
                print(f"  ✓ Pattern: Spiral")
            elif 'Concentric' in region_name:
                print(f"  ✓ Pattern: Concentric circles")
            elif 'No pattern' in region_name:
                print(f"  ✓ Pattern: None (as specified)")
    
    print(f"\n{'='*60}")
    print(f"TOTAL DRAWING LINES: {total_lines}")
    print(f"{'='*60}")
    
    # Verify requirements
    print("\n" + "="*60)
    print("REQUIREMENT VERIFICATION")
    print("="*60)
    
    requirements = {
        'Rectangle 1 (Rectilinear 0°)': False,
        'Rectangle 2 (Honeycomb)': False,
        'Triangle (Spiral)': False,
        'Rect1 ∩ Rect2 (Rectilinear 90°)': False,
        'Rect1 ∩ Triangle (Rectilinear 120°)': False,
        'Rect2 ∩ Triangle (No pattern)': False,
        'Triple Intersection (Concentric)': False,
    }
    
    for region_name in regions.keys():
        # Check for Rectangle 1
        if 'Rectangle 1' in region_name and 'Rectilinear 0' in region_name and 'No pattern' not in region_name:
            requirements['Rectangle 1 (Rectilinear 0°)'] = True
        # Check for Rectangle 2
        elif 'Rectangle 2' in region_name and 'Honeycomb' in region_name:
            requirements['Rectangle 2 (Honeycomb)'] = True
        # Check for Triangle
        elif 'Triangle' in region_name and 'Spiral' in region_name:
            requirements['Triangle (Spiral)'] = True
        # Check for Rect1 ∩ Rect2
        if 'Rect1' in region_name and 'Rect2' in region_name and '90' in region_name:
            requirements['Rect1 ∩ Rect2 (Rectilinear 90°)'] = True
        # Check for Rect1 ∩ Triangle
        if 'Rect1' in region_name and 'Triangle' in region_name and '120' in region_name:
            requirements['Rect1 ∩ Triangle (Rectilinear 120°)'] = True
        # Check for Rect2 ∩ Triangle
        if 'Rect2' in region_name and 'Triangle' in region_name:
            requirements['Rect2 ∩ Triangle (No pattern)'] = True
        # Check for Triple Intersection
        if 'Triple' in region_name and 'Concentric' in region_name:
            requirements['Triple Intersection (Concentric)'] = True
    
    for req, found in requirements.items():
        status = "✓ PASS" if found else "✗ FAIL"
        print(f"{status}: {req}")
    
    all_pass = all(requirements.values())
    print(f"\n{'='*60}")
    if all_pass:
        print("✓ ALL REQUIREMENTS MET!")
    else:
        print("✗ Some requirements missing")
    print(f"{'='*60}\n")


def main():
    """Main function"""
    gcode_file = "output_patterns.gcode"
    
    print(f"Parsing G-code file: {gcode_file}")
    regions = parse_gcode(gcode_file)
    
    print(f"Found {len(regions)} regions")
    
    # Print statistics and verification
    print_statistics(regions)
    
    # Create visualization
    print("\nGenerating visualization...")
    plot_gcode(regions)


if __name__ == "__main__":
    main()
