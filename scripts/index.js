// Function to fetch benchmark data from JSON file and display it in the chart
async function fetchAndDisplayBenchmarkData() {
    try {
        const response = await fetch('benchmark_results.json');
        const benchmarkData = await response.json();

        const labels = benchmarkData.benchmarks.map(benchmark => benchmark.name);
        const cpuTimeData = benchmarkData.benchmarks.map(benchmark => benchmark.cpu_time.toFixed(2));

        //const ctx = document.getElementsByClassName('benchmarkChart')[0].getContext('2d');

        let graphs = new Map();

        for (let i = 0; i < labels.length; i++) {
            let time_point = { title: labels[i].split('_')[1].split('/')[0], iteration: parseInt(labels[i].split('/')[1], 10), time: parseFloat(cpuTimeData[i], 10) };

            let graph_key = labels[i].split('_')[1].split('/')[0];
            if (graphs.has(graph_key)) {
                graphs.get(graph_key).push(time_point);
            } else {
                graphs.set(graph_key, [time_point]);
            }
        }
        console.log(graphs);

        const container = document.getElementById('halfSize');
        container.style.width = window.innerWidth / 2 + 'px';

        const table = document.getElementById("graphTable");
        for (const [graph_title, graph_data] of graphs.entries()) {
            const newRow = table.insertRow(-1);
            const newCell = newRow.insertCell(0);
            const canvas = document.createElement('canvas');
            canvas.width = container.offsetWidth;
            canvas.height = container.offsetHeight;
            canvas.className = "benchmarkChart";
            newCell.appendChild(canvas);

            console.log(graph_data);
            
            // Draw something on the canvas (optional)
            const ctx = canvas.getContext('2d');
            const chart = new Chart(ctx, {
                type: 'line',
                data: {
                    labels: graph_data.map(obj => obj.iteration),
                    datasets: [
                        {
                            label: 'CPU Time (ns)',
                            data: graph_data.map(obj => obj.time),
                            backgroundColor: 'rgba(255, 99, 132, 0.2)',
                            borderColor: 'rgba(255, 99, 132, 1)',
                            borderWidth: 1
                        }
                    ]
                },
                options: {
                    responsive: true,
                    maintainAspectRatio: false,  
                    scales: {
                        y: {
                            beginAtZero: true
                        }
                    }
                }
            });
        }
    } catch (error) {
        console.error('Error fetching or parsing benchmark data:', error);
    }
}

window.addEventListener('load', function () {
    fetchAndDisplayBenchmarkData();
    const divElement = document.getElementById('halfSize');
    divElement.style.width = window.innerWidth / 2 + 'px';
});

window.addEventListener('resize', function () {
    const container = document.getElementById('halfSize');
    container.style.width = window.innerWidth / 2 + 'px';
    Array(document.getElementsByClassName("benchmarkChart")).forEach(canvas => {
        canvas.width = container.offsetWidth;
        canvas.height = container.offsetHeight;
    });
});

