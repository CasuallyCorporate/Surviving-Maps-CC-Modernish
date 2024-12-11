import { useState, useEffect } from 'react'
import parse from "html-react-parser"

import reactLogo from './assets/react.svg'
import survivingLogo from '/Logo.png'

import appMain from './tscomponents/appMain'
import Modal from 'react-modal';

Modal.setAppElement('#Info');

function App() {
  // Header
  // AppBody:{
  // Search Parameters
  // item(s) View
  // Result view
  // }
  // Footer

  const [modalIsOpen, setIsOpen] = useState(false);
  const [modalPageName, setModalPageName] = useState("");
  const [modalLastFetchName, setModalLastFetchName] = useState("");
  const [modalPageContents, setModalPageContents] = useState("");
  const APIURL: string = "/motd";

  async function fetchModPage(apiURL: string, pageStr: string) {
    console.log("fetching Modal");

    if (pageStr == modalLastFetchName) {
      if (pageStr == "") {
        console.log("page name empty. Fetch cancelled");
        return;
      }
      if (modalPageContents != "") {
        console.log("last page == this request, and current contents not empty. Fetch cancelled");
        return;
      }
    }

    const response = await fetch(apiURL,
      { method: "POST",
        headers: {"Content-Type": "application/json"},
        body: JSON.stringify({ page: pageStr })
      }
    );
  
    if(response.status == 200) {
      let pageson = await response.json();
      setModalPageContents(pageson[1].PageData);
      setModalLastFetchName(pageStr);
      console.log("Modal fetch succeeded");
      return;
    }

    if(response.status < 600 || response.status >= 500) {
      // Failed, from server
      setModalPageContents("<h5>Server error</h5");
      setModalLastFetchName("");
      console.log("Modal fetch failed: Server related error");
      return;
    }

    // Fail through
    setModalPageContents("<h5>Unknown error</h5");
    setModalLastFetchName("");
    console.log("Modal fetch failed: Response=: " + response.statusText);
    return;
  }

  function openModal(page: string) {
    setModalPageName(page);
    fetchModPage(APIURL, page);
    setIsOpen(true);
  }

  function afterOpenModal() {
    
  }

  function closeModal() {
    setIsOpen(false);
    setModalPageName("");
  }

  useEffect(() => {
    if(!modalIsOpen) {
      console.log("Modal not open");
      return;
    }

    fetchModPage(APIURL, modalPageName);
  }, [])

  return [
    <>
    <div id="Header" className="row">
      <nav aria-label="navigationDiv" className="navbar navbar-expand-md navbar-dark bg-dark fixed-top">
        <div className="container-fluid">
          <a href="https://react.dev" target="_blank" className="navbar">
            <img src={reactLogo} className="logo react" alt="React logo" />
          </a>
          <p>/</p>
          <a href="https://survivingmars.paradoxwikis.com/Surviving_Mars_Wiki" className="navbar-brand">
            <img src={survivingLogo} className="logo survivingmars" alt="Surviving Mars Logo" />
          </a>
          <a className="navbar-brand" href="/">Surviving Maps.cc</a>
          <button aria-controls="navbarNavDropdown" aria-expanded="false" aria-label="Toggle navigation" className="navbar-toggler"
                    data-bs-target="#navbarNavDropdown" data-bs-toggle="collapse" type="button">
            <span className="navbar-toggler-icon"></span>
          </button>
          <div className="collapse navbar-collapse" id="navbarNavDropdown">
            <ul className="navbar-nav">
              <li className="nav-item" id="Home">
                <a className="nav-link" href="/">Home</a>
              </li>
              <li className="nav-item" id="FAQ">
                <a className="nav-link" onClick={() => openModal("FAQ")}>FAQ</a>
              </li>
              <li className="nav-item" id="Changelog">
                <a className="nav-link" onClick={() => openModal("Changelog")}>Changelog</a>
              </li>
            </ul>
            </div>
        </div>
      </nav>
    </div>
    </>,
    <Modal
      isOpen={modalIsOpen}
      onAfterOpen={afterOpenModal}
      onRequestClose={closeModal}
      >
        <button onClick={closeModal}>close</button>
        <div id='ModalContent'>{parse(modalPageContents)}</div>
    </Modal>
    ,appMain(),
    <>
    <div id="Footer">
      <nav aria-label="footerDiv" className="navbar navbar-expand-md navbar-dark bg-dark fixed-bottom">
        <div className="container-fluid row">
          <div className="collapse navbar-collapse" id="navbown">
            <div className="col-8 text-white">
              This website has no guarantees and has been developed as a hobby project
            </div>
          </div>
        </div>
      </nav>
    </div>
    </>
  ];
}

export default App
