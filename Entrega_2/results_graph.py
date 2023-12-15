import pandas as pd
import matplotlib.pyplot as plt

# Replace 'your_file.csv' with the path to your CSV file
file_path = '/Users/sofiwi/Library/CloudStorage/GoogleDrive-sofia.riquelme@sansano.usm.cl/My Drive/2023 - 02/Proyecto IA/Entregas/Entrega_2/resultados.csv'

# Read the CSV file
df = pd.read_csv(file_path, delimiter=';', dtype=str, encoding='utf-8')

# Replace commas with periods in 'Tiempo {s}' and convert to numeric
df['Tiempo {s}'] = pd.to_numeric(df['Tiempo {s}'].str.replace(',', '.'), errors='coerce')

# Convert 'Cantidad nodos' to numeric and handle non-numeric values
df['Cantidad nodos'] = pd.to_numeric(df['Cantidad nodos'], errors='coerce')

# Drop rows with NaN values in these columns
df.dropna(subset=['Cantidad nodos', 'Tiempo {s}'], inplace=True)

# Convert 'Cantidad nodos' to integer
df['Cantidad nodos'] = df['Cantidad nodos'].astype(int)

# Plot
plt.plot(df['Cantidad nodos'], df['Tiempo {s}'])
plt.yscale("log")
plt.xlabel('Number of Nodes')
plt.ylabel('Time (seconds)')
plt.title('Time vs Number of Nodes')

plt.show()
