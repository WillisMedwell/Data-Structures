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

            let time_point;
            try {
                time_point = { title: labels[i].split(':')[1].split('/')[0], iteration: parseInt(labels[i].split('/')[1], 10), time: parseFloat(cpuTimeData[i], 10) };
            }
            catch {
                time_point = { title: labels[i].split(':')[1].split('/')[0], iteration: parseInt(labels[i].split('/')[1], 10), time: parseFloat(cpuTimeData[i], 10) };
            }

            let graph_key = labels[i].split(':')[0];
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
            const canvas = document.createElement('canvas');
            canvas.width = container.offsetWidth;
            canvas.height = container.offsetHeight;
            canvas.className = "benchmarkChart";
            // const resetZoomButton = document.createElement('button');
            // resetZoomButton.innerText = "Rest Zoom";
            // const enableLogButton = document.createElement('checkbox');
            // //enableLogButton.innerText = "Enable Log Scaling";

            table.insertRow(-1).insertCell(0).appendChild(canvas);

            //let row = table.insertRow(-1);
            //row.insertCell(0).appendChild(resetZoomButton).appendChild(enableLogButton);

            console.log(graph_data);

            const groupedData = graph_data.reduce((acc, obj) => {
                if (!acc[obj.title]) {
                    acc[obj.title] = {
                        iterations: [],
                        times: []
                    };
                }
                acc[obj.title].iterations.push(obj.iteration);
                acc[obj.title].times.push(obj.time);
                return acc;
            }, {});
            
            // Prepare datasets for the chart
            const datasets = Object.keys(groupedData).map((title, index) => {
                const colors = ['rgba(54, 162, 235, 0.5)', 'rgba(255, 99, 132, 0.5)', 'rgba(132, 99, 255, 0.5)']; // Add more colors if needed
                return {
                    label: title,
                    data: groupedData[title].times,
                    borderColor: colors[index % colors.length],
                    fill: false
                };
            });

            // Draw something on the canvas (optional)
            const ctx = canvas.getContext('2d');
            const chart = new Chart(ctx, {
                type: 'line',
                data: {
                    labels: groupedData['Sov'].iterations, // Assumes all datasets have the same iterations
                    datasets: datasets
                },
                options: {
                    responsive: true,
                    maintainAspectRatio: false,
                    scales: {
                        x: {
                            title: {
                                display: true,
                                text: 'Iterations'
                            }
                        },
                        y: {
                            type: 'logarithmic', // Make y-axis logarithmic
                            title: {
                                display: true,
                                text: 'Time (ns)'
                            },
                            ticks: {
                                // Include a dollar sign in the ticks
                                callback: function (value, index, values) {
                                    return value;
                                }
                            }
                        }
                    },
                    plugins: {
                        title: {
                            display: true,
                            text: graph_title,
                            position: 'top',
                            font: {
                                size: 30 // Font size in pixels
                            }
                        },
                        legend: {
                            position: 'right'
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
    //const divElement = document.getElementById('halfSize');
    //divElement.style.width = window.innerWidth / 2 + 'px';
});

// window.addEventListener('resize', function () {
//     const container = document.getElementById('halfSize');
//     container.style.width = window.innerWidth / 2 + 'px';
//     Array(document.getElementsByClassName("benchmarkChart")).forEach(canvas => {
//         canvas.width = container.offsetWidth;
//         canvas.height = container.offsetHeight;
//     });
// });

