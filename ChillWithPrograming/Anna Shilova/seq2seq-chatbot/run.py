import tensorflow as tf
import tensorlayer as tl
from data.twitter import data
from tensorlayer.models.seq2seq import Seq2seq
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QListWidgetItem
from mainwindow import Ui_MainWindow
import sys

import os
os.environ["CUDA_VISIBLE_DEVICES"] = "-1"
 
def initial_setup(data_corpus):
    metadata, idx_q, idx_a = data.load_data(PATH='data/{}/'.format(data_corpus))
    (trainX, trainY), (testX, testY), (validX, validY) = data.split_dataset(idx_q, idx_a)
    trainX = tl.prepro.remove_pad_sequences(trainX.tolist())
    trainY = tl.prepro.remove_pad_sequences(trainY.tolist())
    testX = tl.prepro.remove_pad_sequences(testX.tolist())
    testY = tl.prepro.remove_pad_sequences(testY.tolist())
    validX = tl.prepro.remove_pad_sequences(validX.tolist())
    validY = tl.prepro.remove_pad_sequences(validY.tolist())
    return metadata, trainX, trainY, testX, testY, validX, validY

def init():
    global model_, word2idx, start_id, unk_id, idx2word
    data_corpus = "twitter"

        #data preprocessing
    metadata, trainX, trainY, testX, testY, validX, validY = initial_setup(data_corpus)

        # Parameters
    src_len = len(trainX)
    tgt_len = len(trainY)

    assert src_len == tgt_len

    batch_size = 32
    n_step = src_len // batch_size
    src_vocab_size = len(metadata['idx2w']) # 8002 (0~8001)
    emb_dim = 1024

    word2idx = metadata['w2idx']   # dict  word 2 index
    idx2word = metadata['idx2w']   # list index 2 word

    unk_id = word2idx['unk']   # 1
    pad_id = word2idx['_']     # 0

    start_id = src_vocab_size  # 8002
    end_id = src_vocab_size + 1  # 8003

    word2idx.update({'start_id': start_id})
    word2idx.update({'end_id': end_id})
    idx2word = idx2word + ['start_id', 'end_id']

    src_vocab_size = tgt_vocab_size = src_vocab_size + 2

    num_epochs = 3
    vocabulary_size = src_vocab_size
    decoder_seq_length = 20
    model_ = Seq2seq(
            decoder_seq_length = decoder_seq_length,
            cell_enc=tf.keras.layers.GRUCell,
            cell_dec=tf.keras.layers.GRUCell,
            n_layer=3,
            n_units=256,
            embedding_layer=tl.layers.Embedding(vocabulary_size=vocabulary_size, embedding_size=emb_dim),
            )

    model_.load_weights("model.hdf5")

class MyWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super(MyWindow, self).__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        self.ui.pushButton.clicked.connect(self.button_clicked)
        self.ui.lineEdit.returnPressed.connect(self.button_clicked)
    
    def button_clicked(self):
        sender = self.sender()
        item = QListWidgetItem('Me: ' + self.ui.lineEdit.text())
        self.ui.listWidget.addItem(item)

        sentence = inference(self.ui.lineEdit.text(), 3)
        item = QListWidgetItem('Bot: ' + " ".join(sentence))
        self.ui.listWidget.addItem(item)
        self.ui.lineEdit.clear()

def inference(seed, top_n):
        model_.eval()
        seed_id = [word2idx.get(w, unk_id) for w in seed.split(" ")]
        sentence_id = model_(inputs=[[seed_id]], seq_length=20, start_token=start_id, top_n = top_n)
        sentence = []
        for w_id in sentence_id[0]:
            w = idx2word[w_id]
            if w == 'end_id':
                break
            sentence = sentence + [w]
        return sentence




if __name__ == "__main__":
    init()
    app = QtWidgets.QApplication([])
    application = MyWindow()
    
    application.show() 
    sys.exit(app.exec())
    
    #while True:
        #s = input("Query >")
        #top_n = 3
        #for i in range(top_n):
            #sentence = inference(s, top_n)
            #print(">", ' '.join(sentence))
