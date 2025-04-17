import turtle

# Set up the screen
wn = turtle.Screen()
wn.bgcolor("black")

# Create a turtle
t = turtle.Turtle()
t.shape("turtle")
t.color("green")
t.speed(2)

# Animation loop
for i in range(36):
    t.forward(100)
    t.right(170)

# Hide the turtle and display the window
t.hideturtle()
wn.mainloop()