const express = require('express');
const cors = require('cors');

const app = express();
app.use(cors());
app.use(express.json());

app.post('/fall-alert', (req, res) => {
  const data = req.body;
  console.log('Dữ liệu nhận được:', data);
  res.status(200).send({ message: 'Fall alert received!' });
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
  console.log(`Server running on port ${PORT}`);
});
