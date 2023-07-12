# API VRP Google.
import os
import googlemaps
import pandas as pd
import numpy as np
from dotenv import load_dotenv;

load_dotenv();

def create_data():
    """Creates the data."""
    data = {}
    # read api key from .env file
    data['API_key'] = os.getenv('GOOGLE_API_KEY');
    data['addresses'] = [
        'Oregon+Foods+Planta',
        'Av.+Javier+Prado+Este+2010,+Lima+15036',  # La rambla
        'C.+Los+Libertadores+302,+San+Isidro+Lima+15073', # Long Horn San Isidro
        'Av.+Elmer+Faucett,+Lima+07001',  # Makro Callao
        'Jr.+Ucello+162,+San+Borja+Lima+15036',  # Wong San Borja
    ]
    data['num_vehiculos'] = 6
    return data

def get_distances(data):
    gmaps = googlemaps.Client(key=data['API_key'])
    addresses = data['addresses']
    matrix = []

    for i in range(len(addresses)):
        row = []
        for j in range(len(addresses)):
            if i == j:
                row.append(0)
            else:
                origin = [addresses[i]]
                destination = [addresses[j]]
                result = gmaps.distance_matrix(origin, destination)
                if result['rows'][0]['elements'][0]['status'] == 'OK':
                    row.append(result['rows'][0]['elements'][0]['distance']['value'])
                else:
                    row.append(np.inf)
        matrix.append(row)
    
    return matrix

def main():
    data = create_data()
    distance_matrix = get_distances(data)
    
    # Guardar en .csv
    df = pd.DataFrame(distance_matrix);
    df.to_csv('./data/db/distance_matrix.csv');
    
    # Guardar en .txt
    with open('./data/db/distance_matrix.txt', 'w') as file:
        for row in distance_matrix:
            file.write(' '.join([str(elem) for elem in row]) + '\n');
            file.write('\n');
        file.close();

if __name__ == '__main__':
    main()