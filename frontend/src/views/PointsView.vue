<template>
    <div>
        <input v-model="address" placeholder="Enter an address" />
        <button @click="searchAddress">Buscar</button>
        <ul v-if="suggestions.length">
            <li v-for="suggestion in suggestions" @click="selectSuggestion(suggestion)">{{ suggestion.description }}</li>
        </ul>
        <button @click="submitLocations">Submit</button>
        <div v-for="location in locations" :key="location">
            {{ location }}
        </div>
    </div>
</template>
  
<script>
import axios from 'axios';

export default {
    data() {
        return {
            address: '',
            locations: [],
            suggestions: [],
        };
    },
    methods: {
        searchAddress() {
            axios.get(`http://localhost:4040/autocomplete/${this.address}`)
                .then(response => {
                    this.suggestions = response.data.predictions;
                });
        },
        selectSuggestion(suggestion) {
            this.address = suggestion.description;
            this.suggestions = [];
        },
        submitLocations() {
            this.locations.push(this.address.replace(/ /g, '+'));
            this.address = '';
            axios.post('http://localhost:4040/submit', { locations: this.locations })
                .then(response => {
                    console.log(response.data);
                });
        },
    },
};
</script>