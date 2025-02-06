def draw_8x8_grid_with_clean_pe_borders():
    fig, ax = plt.subplots(figsize=(6, 6))

    # Remove axis limits and ticks
    ax.set_xticks([])
    ax.set_yticks([])
    ax.set_frame_on(False)  # Remove outer frame

    # Define spacing factor
    spacing = 0.05  # Small gap between PE cells

    # Draw independent PE cells with borders only
    for row in range(8):
        for col in range(8):
            hex_number = format(row * 8 + col, 'X')

            # Draw independent PE cells with spacing and borders
            ax.add_patch(plt.Rectangle((col + spacing, 7 - row + spacing), 
                                       1 - 2 * spacing, 1 - 2 * spacing, 
                                       edgecolor='black', facecolor='white', linewidth=0.8))
            
            # Add text label
            ax.text(col + 0.5, 7.5 - row, hex_number, 
                    color='blue', fontsize=10, ha='center', va='center')

    # Adjust axis limits based on spacing
    ax.set_xlim(0, 8)
    ax.set_ylim(0, 8)

    # Remove axis lines
    ax.axis('off')

    # Add title
    ax.set_title("PE allocation in MN-Core", fontsize=14)
    plt.show()

# Draw the 8x8 grid with clean PE borders
draw_8x8_grid_with_clean_pe_borders()
