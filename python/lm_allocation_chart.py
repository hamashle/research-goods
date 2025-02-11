def draw_6x6_grid_with_lm_numbers():
    fig, ax = plt.subplots(figsize=(6, 6))
    
    # Create grid lines
    for x in range(7):
        ax.axhline(y=x, color='black', linewidth=0.8)
        ax.axvline(x=x, color='black', linewidth=0.8)
    
    # Set axis limits and remove ticks
    ax.set_xlim(0, 6)
    ax.set_ylim(0, 6)
    ax.set_xticks([])
    ax.set_yticks([])
    ax.set_aspect('equal')

    # Add "lm" labels with even numbers to each cell
    for row in range(6):
        for col in range(6):
            even_number = row * 12 + col * 2
            lm_label = f"lm{even_number}"
            ax.text(col + 0.5, 5.5 - row, lm_label, 
                    color='blue', fontsize=10, ha='center', va='center')

    # Add the title
    ax.set_title("Local memory allocation in PE", fontsize=14)
    plt.show()

# Draw the grid with "lm" labels
draw_6x6_grid_with_lm_numbers()
